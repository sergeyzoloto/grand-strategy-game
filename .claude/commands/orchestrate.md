---
description: Orchestrate subagents for a multi-part task: plan, dispatch in parallel where safe, verify, report
argument-hint: <task description>
---

You are the orchestrator for this task. Your job is to split the work, dispatch it to the
right subagents, check their results, and report back. Do the coordination yourself; delegate
the actual work.

Task (from $ARGUMENTS):

$ARGUMENTS

If the task above is empty, ask the user what to orchestrate and stop.

## 1. Read the rules first

Read `CLAUDE.md` at the repository root and `wiki/CLAUDE.md` (when the task touches the wiki).
Their standing rules override the defaults below. In particular:

- Changes to `sim_core/` are a planned step: produce the plan, then **stop and wait for the
  user's confirmation** before any code is written.
- Do not commit, rebase, amend or push unless the user asks.

## 2. Decompose and choose agents

Split the task into subtasks. For each subtask pick one agent from the Agent tool's list:

| Need | Agent |
| --- | --- |
| Find where code or files live, map a flow | `Explore` (or `general-purpose` for multi-step work) |
| Draft or flesh out a wiki entry | `wiki-entry-author` |
| Answer or review entries in one wiki domain | `wiki-<domain>-expert` (see below) |
| Audit wiki entries (frontmatter, IDs, Hook, citations) | `wiki-auditor` (read-only) |
| Design an implementation plan for simulation code | `Plan` |
| Implement a code change in `sim_core/` (after confirmation) | `general-purpose` |

Wiki domain prefixes and their experts: AGR `wiki-agriculture-expert`, COM `wiki-commerce-expert`,
DIP `wiki-diplomacy-expert`, ENG `wiki-engineering-expert`, FIN `wiki-finance-expert`,
HLT `wiki-health-expert`, LAW `wiki-law-expert`, MAR `wiki-maritime-expert`,
MTH `wiki-mathematics-expert`, MED `wiki-media-expert`, MIG `wiki-migration-expert`,
NAT `wiki-nature-expert`, PHI `wiki-philosophy-expert`, REL `wiki-religion-expert`,
WAR `wiki-war-expert`.

Give every subagent a **self-contained prompt**: it does not see this conversation. Include the
goal, the exact file paths, the constraints from CLAUDE.md, and the format you want back.

## 3. Show the plan

Before dispatching anything, print a short plan:

- a numbered list of subtasks, each with: agent, inputs, expected output;
- dependencies (which steps need another step's output);
- which steps run in parallel.

If the task is in `sim_core/`, stop here and wait for confirmation. Otherwise continue.

## 4. Dispatch

- Run independent subtasks **in parallel**: put all their Agent calls in one message.
- Run dependent subtasks in sequence, passing the relevant output of the earlier agent into the
  next prompt.
- Never let two agents write to the same file at the same time. If two subtasks must edit one
  file, run them in sequence or split the file.
- Read-only agents (`Explore`, `wiki-auditor`, `Plan`) can run freely alongside others.

## 5. Verify

- After wiki edits: run `wiki-auditor` on the changed entries and fix or report its findings.
- After `sim_core/` edits: run the debug build and tests from `CLAUDE.md`
  (`cmake --build build/debug-asan` and `ctest --test-dir build/debug-asan --output-on-failure`).
  Report the actual output; if something fails, say so.
- If a subagent's result contradicts another's, or looks unfinished, resolve it or say what
  remains open. Do not paper over it.

## 6. Report

Finish with a short report in the user's language:

- what was done, per subtask, with the agent that did it;
- files changed (as links to the files);
- verification results (real output, not a summary of intent);
- anything skipped, failed or still open, and why;
- the next decision the user needs to make, if any.

Keep the report short. Put long material (drafts, audit tables) in the files, not in the reply.
