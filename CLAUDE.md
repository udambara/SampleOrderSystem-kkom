# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Git workflow

Never run `git commit` or `git push` without first showing the user the change and getting explicit go-ahead in that conversation, even if a previous turn approved a commit/push — approval does not carry over to later changes. This applies regardless of how confident the change looks.

## Project status

This repository currently contains **design/planning documents and an empty Visual Studio project skeleton only** — no application source code (`.cpp`/`.h`) exists yet. Before writing code, read `docs/PRD.md` and the relevant `docs/phases/phaseN.md` file for the phase being implemented; do not invent requirements that aren't in these docs.

## Build

- Solution file: `ProjectSampleOrderSystem.slnx`; project: `ProjectSampleOrderSystem/ProjectSampleOrderSystem.vcxproj`
- C++20, Windows console application (`ConfigurationType=Application`, `SubSystem=Console`), built with MSVC (`PlatformToolset v145`)
- Configurations: `Debug`/`Release` × `Win32`/`x64`
- Build via Visual Studio, or from a Developer Command Prompt:
  ```
  msbuild ProjectSampleOrderSystem.slnx /p:Configuration=Debug /p:Platform=x64
  ```
- No test framework or lint tooling is configured yet. Test verification is currently manual — see `docs/test/test_planN.md` for the step-by-step scenarios to run against the built executable for each phase.

## Origin and reference implementations

This project is a from-scratch reimplementation of a prior PoC that was split across separate repos. Those repos are read-only references, not dependencies to build against — do not clone or wire them into the build. When a phase doc references logic "from the PoC," the reference repos are:

- `udambara/poc-SampleOrderSystem` — original integrated PoC
- `udambara/ConsoleMVC-kkom` — console MVC UI reference
- `udambara/DataPersistence-kkom` — JSON persistence reference (includes `nlohmann/json.hpp`)
- `udambara/DataMonitor-kkom` — monitoring reference
- `udambara/DummyDataGenerator-kkom` — dummy data generator reference

## Architecture (planned — see docs/PRD.md and docs/phases/)

The system is a single-process console app for managing semiconductor sample orders, structured in layers (originally separate repos in the PoC, now consolidated into one project):

- **ConsoleMVC** — console UI. `AppController` runs the main menu loop and calls `OrderWorkflow::Tick()` once per iteration; `ConsoleView` handles raw input/output; one `*Controller` per main menu item does input validation and delegates to DataPersistence/DataMonitor. Controllers should not contain business logic — that belongs in `OrderWorkflow`/repositories.
- **DataPersistence** — `Sample`/`Order` domain structs (JSON-serializable via `to_json`/`from_json`), `SampleRepository`/`OrderRepository` (JSON file-backed CRUD, single-process, no concurrency handling), and `OrderWorkflow` (approve/reject/tick — the core state-machine logic).
- **DataMonitor** — read-only reporting over the repositories (order counts by status, inventory levels); must never mutate data.
- **DummyDataGenerator** — separate executable/utility that seeds the same JSON data files with random Sample/Order data for testing.
- **Logging** — a `Logger` utility, introduced in Phase 1, that every order status transition (in normal flow and in admin mode) must call. Log target: `logs/order_events.log`.

### Order state machine

`Order.status` moves through `RESERVED → CONFIRMED|PRODUCING → CONFIRMED → RELEASE`, or `RESERVED → REJECTED`. The full field list, transition rules, and production-time math (yield, shortage quantity, FIFO production queue) are defined in `docs/PRD.md` under "도메인 모델" and "기능 요구사항" — read that before touching `OrderWorkflow`.

### Menu 8 — admin mode

Menu 8 is a test-acceleration path (see `docs/phases/phase9.md`): it creates an order that skips the RESERVED/approval step and enters the production queue directly with an operator-specified `totalProductionSeconds`, reusing `OrderWorkflow`'s existing queueing logic (`IsLineBusy`, `Tick`) rather than duplicating it.

## Implementation plan structure

- `docs/phases/phase1.md` … `phase9.md` — detailed per-phase design (data, behavior, files, completion criteria). Phase order is fixed by dependency: 1 (foundation) → 2 (sample mgmt) → 3 (ordering) → 4 (approval) → 5 (production) → 6 (shipping) → 7 (monitoring) → 8 (dummy data generator) → 9 (admin mode).
- `docs_temp/plan1.md` … `plan9.md` — condensed checklist version of each phase doc, meant for tracking implementation progress.
- `docs/test/test_plan1.md` … `test_plan9.md` — manual test scenarios (TCx-y) per phase, including log-verification steps for phases that touch order state transitions.

When implementing a phase, treat its `docs/phases/phaseN.md` as the source of truth for scope, and its `docs/test/test_planN.md` as the acceptance check before moving to the next phase.
