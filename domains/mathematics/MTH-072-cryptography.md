---
id: MTH-072
title: Cryptography and Security from Mathematical Structure
legacy_ref:
domain: mathematics
domains_touched: [media, war, diplomacy, commerce, law]
tier: core
status: drafted
confidence: medium
date_start: 1883
date_end: 2000
date_precision: decade
regions: [middle-east, europe, americas, east-asia]
region_variants:
  - region: middle-east
    date_start: 850
    note: Al-Kindi's treatise on deciphering around 850 is the earliest known systematic cryptanalysis, using the frequency of letters in Arabic to break substitution ciphers, and the tradition continues through Ibn Adlan and Ibn al-Durayhim to al-Qalqashandi's chancery encyclopedia of 1412, which sets out both enciphering methods and their breaking for administrative use. This is the foundation the entry rests on — the demonstration that a secret method yields to statistics — and it is sustained as working bureaucratic practice, not a curiosity.
  - region: europe
    date_start: 1883
    note: Kerckhoffs states in 1883 that a military cipher must remain secure even if the enemy knows the whole system except the key, which moves security from secrecy of method to secrecy of a changeable key. In the 1930s the Polish Cipher Bureau recruits mathematicians rather than linguists, and Rejewski reconstructs the Enigma's wiring with permutation theory in 1932, work handed to Britain in 1939 and extended at Bletchley Park — the moment cryptanalysis becomes applied mathematics.
  - region: east-asia
    date_start: 1940
    note: Japanese diplomatic and naval ciphers rested substantially on the complexity and secrecy of their machines and codebooks, and American cryptanalysts broke the diplomatic system known as Purple in 1940 and read enough naval traffic to anticipate the attack on Midway in 1942. The case shows the older model of security failing at the highest stakes — a system judged safe because its method was hidden, defeated by analysis of its traffic.
  - region: americas
    date_start: 1949
    note: Shannon's 1949 paper on secrecy systems proves that a one-time pad is perfectly secure and gives cryptography its mathematical theory. Diffie and Hellman in 1976 publish a method by which two parties can agree a secret over an open channel, and Rivest, Shamir and Adleman in 1977 publish public-key encryption resting on the difficulty of factoring. The United States then classifies strong cryptography as a munition and restricts its export until the late 1990s.
  - region: europe
    date_start: 1970
    note: At GCHQ, James Ellis conceives non-secret encryption in 1970 and Clifford Cocks devises what is essentially RSA in 1973, years before the American publications. The work stays classified until 1997, so the public priority went to others — the same erasure of priority by secrecy that Colossus (MTH-065) records, repeated a generation later in the same institution's lineage.
absence:
  - region: central-asia-steppe
    note: Steppe empires sent their most sensitive messages by trusted envoys under authorising tablets (MED-030), frequently memorised rather than written, so there was little enciphered writing to intercept. Where secrecy is held in a messenger's memory and loyalty rather than in text, no adversary accumulates intercepted material, and without intercepted material there is no pressure to develop cryptanalysis or the stronger systems it forces. What is absent is the written secret correspondence at scale that the whole contest depends on.
prereqs: [MTH-040, MTH-025]
enables: [MED-150]
see_also: [MTH-090, MTH-065, MAR-340, DIP-110, MED-120, MTH-080]
contested: true
contested_note: "Three disputes. First and most important, the claim that security rests on mathematical structure is only partly a guarantee. Shannon proved the one-time pad perfectly secure, but public-key systems rest on the assumed difficulty of problems such as factoring large numbers, and no one has proved that difficulty — it is conjecture tied to open questions in computational complexity, and Shor's 1994 algorithm shows factoring would be easy on a sufficiently large quantum computer. Model public-key security as conditional on hardness assumptions that could fail, not as proved. Second, priority for public-key cryptography is split between the classified GCHQ work of 1970-73 and the public work of 1976-77; model both as independent inventions, with public credit shaped by secrecy. Third, whether al-Kindi's treatise is the first rests partly on lost texts, since an earlier Arabic work on cryptography by al-Khalil is recorded but does not survive."
stats_touched: [information sovereignty, latent capability]
tags: [cryptography, al-kindi, kerckhoffs, enigma, public-key, rsa, gchq, crypto-wars]
---

Security that comes from mathematical structure rather than from keeping the
method secret. Everything about the method can be public and the message still
unreadable — and, since 1976, two strangers can agree a secret over a channel
everyone can read.

## Timing

From al-Kindi's cryptanalysis around 850 and the Arabic chancery tradition to
1412, through Kerckhoffs's principle of 1883, the Polish and Bletchley
mathematisation of cryptanalysis from 1932, Shannon's theory of 1949, and
public-key cryptography from 1976, to the end of American export controls
around 2000.

## Mechanism

**Cryptanalysis forces security out of the method and into the key.** Al-Kindi
showed around 850 that a substitution cipher leaks the letter frequencies of
its language, so a hidden method is broken by statistics (MTH-040) no matter
how cleverly it is hidden. Every later advance follows from taking that
seriously. Kerckhoffs's principle of 1883 draws the conclusion: assume the
enemy knows the system, and put all security in a small, changeable key. A
captured machine or a leaked manual then costs nothing, because the key is
what matters and it can be replaced.

**Mathematics then becomes the guarantee rather than the weapon against
secrecy.** Shannon proved in 1949 that a one-time pad, a truly random key as
long as the message used once, cannot be broken by any amount of computation.
Public-key cryptography solves the problem that had constrained every system
before it — the key itself had to be delivered secretly. Diffie and Hellman in
1976 showed two parties can agree a shared secret over an open channel, and
RSA in 1977 built encryption on the ease of multiplying large primes and the
difficulty of factoring their product. Security is now a property of a
mathematical relationship that anyone may inspect.

**This is the domain's strongest case of deferred applicability, and it meets
the guard on `latent capability` exactly.** Number theory — Euclid's primes,
Fermat's little theorem of 1640, Euler's generalisation — had no known
application at the time of its derivation, and G. H. Hardy wrote in 1940 that
no warlike purpose had been found for the theory of numbers. That statement is
the evidence the capability was latent by the producing moment's own knowledge,
not by hindsight. Within four decades it secured military communications and
then global commerce.

## Social consequence

Mathematicians become state security personnel. The Polish Cipher Bureau's
decision to hire mathematicians, and Bletchley's recruitment of them, moved
cryptanalysis from a craft of linguists to a discipline of mathematics, and put
people trained in pure theory inside intelligence institutions with all the
secrecy that entails. Secrecy then shapes whose work is remembered. GCHQ's
public-key work of 1970-73 stayed classified until 1997, so credit went to the
later public inventors — the pattern of Colossus (MTH-065) repeated.

Once strong encryption is public mathematics, the contest over it becomes
political. A state that could previously read its own population's and its
adversaries' communications given enough effort now faces messages no effort
will read. `information sovereignty` is directly at stake. The United States
treated strong cryptography as a munition and restricted its export into the
late 1990s, and proposals for government-held keys followed — the state trying
to reclaim, by law, a capacity mathematics had removed.

## Regional specifics

The Arabic tradition is the entry's foundation, not a prelude: systematic
cryptanalysis from al-Kindi through al-Qalqashandi's chancery encyclopedia,
sustained as administrative practice for centuries, established the fact every
later system responds to. The Polish Cipher Bureau made cryptanalysis
mathematical before Bletchley scaled it. The breaking of Japanese diplomatic and
naval systems shows the old security-by-hidden-method model failing at the
highest stakes. Public-key cryptography was invented twice, secretly at GCHQ and
publicly in the United States. The steppe absence records the contest's
precondition as a negative: where secrets travel in trusted memory rather than
in writing, nothing accumulates to intercept, and no cryptanalytic pressure
arises.

## Contested

The mathematical guarantee is partial. The one-time pad is proved secure;
public-key systems rest on the assumed hardness of problems like factoring,
which is unproved, and Shor's 1994 algorithm shows a large quantum computer
would break it. Model public-key security as conditional on hardness
assumptions. Priority is split between GCHQ's classified work and the later
public inventions. Al-Kindi's primacy depends partly on lost earlier texts.

## Hook

Moves security from secrecy of method to secrecy of key, forced by
cryptanalysis showing that hidden methods leak to statistics. Public-key methods
should let two parties agree a secret over an open channel, and their strength
should be conditional on unproved hardness assumptions that a later capability
can break, rather than guaranteed. It should draw directly on `latent capability`
— number theory with no known use at its derivation paying out centuries later —
and remain the canonical case that a result's future value was invisible at the
time. Strong public encryption should reduce `information sovereignty`, pushing
states to reclaim by law and export control what mathematics removed. And
secrecy inside security institutions should erase priority, so public credit
goes to later, open inventors.
