---
id: MTH-087
title: The Stored-Program Architecture
legacy_ref:
domain: mathematics
domains_touched: [engineering, war, commerce, law]
tier: core
status: drafted
confidence: high
date_start: 1945
date_end: 1960
date_precision: decade
regions: [americas, europe, oceania, east-asia, south-asia]
region_variants:
  - region: americas
    date_start: 1945
    note: The First Draft of a Report on the EDVAC, circulated in 1945 under von Neumann's name, describes a machine holding its instructions in the same memory as its data, drawing substantially on the ENIAC team's work. The report's wide circulation spread the design faster than any machine did, and the Institute for Advanced Study machine built to it was used for hydrogen-bomb calculation — the demand that paid for the architecture in the United States.
  - region: europe
    date_start: 1948
    note: The Manchester Small-Scale Experimental Machine ran the first stored program in June 1948, and EDSAC at Cambridge in 1949 became the first to give a regular computing service. EDSAC's team then built the first subroutine library and an assembler-like loader, which shows the architecture's consequence arriving almost immediately — once a program is data, programs that prepare other programs are the obvious next thing to write.
  - region: europe
    date_start: 1950
    note: MESM, built in Kiev under Sergei Lebedev and running from 1950, was designed independently of the Anglo-American machines and was the first stored-program computer in continental Europe. It establishes that the architecture was reached by more than one route under comparable demand rather than transmitted from a single source.
  - region: oceania
    date_start: 1949
    note: CSIRAC, built by CSIR in Sydney and running in 1949, was among the first handful of stored-program computers anywhere and the fourth or fifth by most counts. It is the case that places the architecture's origin outside the North Atlantic, and it later became the first computer to play digital music.
  - region: east-asia
    date_start: 1956
    note: FUJIC, completed in 1956 at Fuji Photo Film essentially by one engineer, Okazaki Bunji, to perform lens-design calculations, was Japan's first stored-program computer. It was built for industrial optics rather than weapons or cryptanalysis, which makes it the clearest early case of the architecture paid for by a civilian commercial demand.
  - region: south-asia
    date_start: 1960
    note: TIFRAC, built at the Tata Institute of Fundamental Research in Bombay and commissioned in 1960, was India's first indigenously built stored-program computer, designed and constructed at a research institute rather than acquired, and used for scientific computation there until imported machines displaced it.
absence:
  - region: africa
    note: No stored-program computer was built on the continent in this window. Every early machine elsewhere was paid for by an urgent sponsor — nuclear weapons calculation, cryptanalysis, ballistics, weather, or in Japan's case industrial optics — and built by an existing electronics base able to supply thousands of valves and a maintenance staff. Neither condition was present, and the first computers to arrive in the following decade were imported, so the architecture came to the region as a finished product rather than as something built to meet a local demand.
prereqs: [MTH-065, MTH-080]
enables: [MED-150]
see_also: [MTH-060, ENG-330, WAR-330, MTH-090, MTH-020, MTH-084]
contested: true
contested_note: "Two disputes. Attribution of the architecture is contested: the 1945 EDVAC report bears von Neumann's name alone and draws on Eckert and Mauchly's work with the ENIAC team, and the naming of the design after von Neumann has been criticised as erasing their contribution, while Turing's contemporaneous ACE design and Zuse's work complicate it further. Model the architecture as emerging from a small, interconnected group and several independent sites rather than from a single author. Second, claims to be the first stored-program computer depend on definition — first to run a stored program (Manchester, 1948), first to offer a regular service (EDSAC, 1949), first built to the EDVAC design, first in continental Europe (MESM) — and should be treated as definitional rather than factual disputes."
stats_touched: [execution delegability, capital specificity]
tags: [stored-program, edvac, manchester-baby, edsac, mesm, csirac, von-neumann-architecture]
---

Instructions and data occupy the same memory, so a program is itself data.
Everything that later runs on computers — assemblers, compilers, operating
systems — is a program that reads, writes or transforms other programs, and
none of it is possible until this is true.

## Timing

From the EDVAC report of 1945, through the Manchester machine's first stored
program in 1948 and EDSAC's service in 1949, CSIRAC in 1949, MESM in 1950,
FUJIC in 1956 and TIFRAC in 1960, by which point the architecture is standard.

## Mechanism

**The procedure stops being held apart from the data.** MTH-065's machines held
their instructions in cards, tape, switches or cables, separate from the
numbers they worked on — to change the task, someone rewired, re-plugged or
loaded a new tape. Here the instructions are numbers in the same memory as
everything else. Changing the task means writing different numbers, which a
program can do.

**So programs can operate on programs, and that is the whole consequence.** An
assembler is a program that turns symbolic instructions into machine
instructions. A compiler turns a higher-level language into a program. A
loader places programs in memory; an operating system manages which programs
run. EDSAC's team built a subroutine library and a loader within a couple of
years of the machine working, because once a program is data, writing
programs that prepare other programs is the obvious step. MTH-080's universal
machine states the same property abstractly in 1936; this is where it becomes
a machine anyone could program.

**The capital stops specialising.** A difference engine tabulates, a tabulator
counts cards, a firing-table machine computes firing tables. A stored-program
computer does whatever the program in its memory does, so `capital
specificity` falls toward zero — the same machine is a payroll system in the
morning and a physics simulation at night. That in turn makes the procedure
separable from the machine as an object in its own right, which is what later
allows software to be written, sold and owned independently of hardware.

**`execution delegability` extends to the construction of procedures
themselves.** MTH-020 made following a procedure delegable; MTH-065 removed the
person from following it; here writing parts of the procedure is delegated to
other procedures.

## Social consequence

Programming becomes a job, and it is at first classified as clerical. The
people who first programmed these machines were frequently drawn from the
human computing pools, and substantially women, because preparing instructions
looked like the tabulation work that preceded it. As programming came to be
seen as intellectually demanding it was reclassified as engineering, its status
and pay rose, and its workforce changed — the pattern MTH-060 records, repeated
one step later.

The architecture arrives inside state programmes and under Cold War demand —
hydrogen-bomb calculation, cryptanalysis, weather prediction — so the first
institutions of computing are military and scientific establishments. FUJIC is
the exception worth marking: built for lens design at a photographic firm, it
shows a civilian industrial demand sufficient on its own.

## Regional specifics

The architecture was reached at several independent sites within a few years
of each other, which argues against a single line of transmission. Manchester
and Cambridge ran the first programs and services; Lebedev's MESM in Kiev was
designed independently; CSIRAC in Sydney was among the first handful anywhere;
FUJIC in Japan was built by one engineer for a commercial purpose; TIFRAC in
Bombay was designed and built indigenously. The EDVAC report's circulation
spread the design faster than any machine, which is why the attribution
dispute matters to who is remembered. Africa is the genuine absence: no sponsor
and no electronics base, so the architecture arrived as an imported product in
the following decade.

## Contested

The architecture's naming after von Neumann is disputed as erasing Eckert and
Mauchly's contribution, and Turing's ACE and Zuse complicate it further; model a
small interconnected group and several independent sites. Claims to be first
are definitional — first stored program run, first regular service, first on
the continent — and should not be treated as factual contests.

## Hook

Puts the procedure in the same memory as the data, so programs can read, write
and transform programs, and the machine's task changes by writing numbers
rather than by rewiring. That should unlock a recursive tier of capability —
assemblers, compilers, operating systems — that no earlier computing entry can
reach. `capital specificity` falls toward zero because one machine does
whatever its memory holds, which separates the procedure from the machine as an
object that can later be written, sold and owned on its own. And it should be
reached independently wherever an urgent sponsor meets an electronics base,
with programming recruited from the computing workforce under a clerical
classification and later reclassified upward.
