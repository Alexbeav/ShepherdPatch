# ShepherdPatch Development Log

This log records the investigation and implementation work from 2026-08-11 through 2026-08-12.

## 2026-08-11: Source review

The work started with a review of ShepherdPatch and UnknownProject 3.20.

ShepherdPatch used source-level runtime hooks and preserved the original game binaries. UnknownProject replaced core binaries and changed some game rules.

The review established a firm boundary. ShepherdPatch fixes and UnknownProject work remain separate.

The UnknownProject repository contained a large binary distribution. Its `v320-src` release contained the useful Delphi source for `shv.dll`.

## 2026-08-11: Configurator path fault

The released configurator searched a fixed path under `%ProgramFiles(x86)%`. This path usually resolved to the `C:` drive.

The **Open game Bin** command also created the guessed directory. An access error terminated the configurator.

The first local repair added a directory picker. It stored the selected Bin directory under `%LocalAppData%\ShepherdPatch`.

The repair also stopped the configurator from creating a guessed game directory. The open command became unavailable without a valid target.

## 2026-08-11: Configurator redesign

The first redesigned window required too much vertical space. Several controls were not visible without a maximized window.

The next layout used responsive cards, category filters, and a persistent footer. The basic view showed only common controls.

The advanced view retained the full runtime configuration. A **Run** command appeared next to **Save**.

Later review added a compact footer, a dark checkbox style, and safe handling for unsaved changes.

The final review also added clear errors for an inaccessible `ShepherdPatch.ini` file.

## 2026-08-11: Frame-rate investigation

The original frame-rate option changed the loading screen but not gameplay. Gameplay remained at 30 FPS.

The runtime still used a stock engine interval near 33.33 milliseconds. The frame-pacing hooks did not replace that engine value.

A guarded engine patch corrected the interval. Gameplay then ran at 60 FPS and felt substantially smoother.

The menu remained at 30 FPS. This behavior matched the desired state.

An early build also accelerated prerendered movies to double speed. The Bink path then received separate timing behavior.

The corrected build kept gameplay at 60 FPS and restored normal movie speed.

## 2026-08-11: High-resolution panel correction

High-resolution gameplay rendered correctly, but maps and notes used only part of the screen. Text used the correct canvas.

The first correction overcompensated and made maps and notes too large. The next correction used resolution-specific panel constants.

The validated build showed full maps, notes, and item images. Text and menus remained correct.

Automated review later found an intermediate-resolution regression. The unified profile table now preserves the established 1600x900 item scale.

## 2026-08-11: Startup and shutdown investigation

Some direct launches stopped before the logos. Some Steam launches also stopped, so Steam was not the complete solution.

The runtime logs and hang captures indicated fragile startup ordering, device creation, and teardown behavior.

The patch gained safer Direct3D retry parameters and startup guards. Steam launch remained an optional configurator choice.

Four sequential launches worked after these changes. This result improved confidence but did not complete a long stability test.

Mouse input during logos remained risky. Focus loss near the menu also exposed frozen visual updates.

## 2026-08-11: Keyboard-prompt investigation

The PC game showed generic action icons instead of keyboard keys. This problem made adjacent actions difficult to distinguish.

The stock localization data contained command tokens. The active input profile contained the necessary keyboard and mouse bindings.

The first prototype replaced tokens with static text labels. It produced prompts such as `[LMB] Open` and `[Q] Inventory`.

An Inventory remap exposed the static design fault. The visible prompt did not change from `Q` to `Tab`.

The next prototype read the live binding table. The prompt then changed after the in-game remap without a restart.

Tests also covered mouse buttons, movement labels, aliases, unknown tokens, and live scan-code overrides.

Some actions still used generic icons during later tests. The prototype therefore remains outside the stable pull requests.

## 2026-08-11: Controller and automatic-mode experiment

The game required a manual input-mode selection. This behavior limited handheld and Steam Deck use.

An experimental hook detected keyboard, mouse, and XInput activity. It changed the engine input selector after meaningful activity.

The first test showed only partial controller operation. Map, selection wheels, inventory toggles, and vertical right-stick movement worked.

Movement, triggers, and most controller actions did not work. The engine therefore stores more controller state than one selector byte.

This experiment remains disabled by default. The next investigation must capture the complete manual mode transition.

## 2026-08-11: Laptop package test

The uploaded GitHub package started on the laptop. The first save load stopped near completion.

The second save load worked. This result validated package portability but retained a startup concern.

## 2026-08-11: Upstream pull requests

Two focused pull requests replaced one large mixed change.

Runtime pull request #1 contains frame-rate, high-resolution UI, logging, and startup fixes.

Configurator pull request #2 contains path selection, responsive layout, and launch improvements.

This split keeps native risk separate from user-interface changes. It also gives the maintainer smaller review units.

## 2026-08-12: Automated-review follow-up

CodeRabbit and Cubic found valid edge cases in both pull requests.

The runtime follow-up corrected these problems:

- Successful device retries now retain the parameters that created the device.
- Borderless retries now fill zero dimensions before the retry.
- The stock frame interval requires an exact 30 FPS value within a small tolerance.
- The high-resolution patch obtains all write permissions before its first write.
- Panel and item constants now use one coordinated profile table.
- The log uses one persistent stream and flushes each line.
- Tests cover values above 4K and non-default timer delays.

The configurator follow-up corrected these problems:

- Value grids now wrap into their declared columns.
- A selected Bin directory takes priority over an old configuration path.
- A valid Bin directory must contain `SilentHill.exe`.
- Steam launch never falls back to direct launch without notice.
- Preference-write errors appear in the user interface.
- A directory switch protects unsaved changes.
- The compact footer keeps all action controls visible.
- The Steam checkbox uses the dark application style.
- Configuration-save errors no longer terminate the application.

Both pull requests passed this review stage. The maintainer later merged both requests.

## 2026-08-12: Second merge-gate review

A second independent review repeated all documented builds and tests. It also examined the final code paths.

The review found a late Direct3D installation window. The engine can call `Direct3DCreate9` before the worker thread patches its import.

Runtime commit `0720ab2` now patches the executable `LoadLibraryA` import during DLL attachment. This hook intercepts the engine load before control returns.

CodeRabbit then found a configuration-publication race in this early path. The first device hook can read `g_config` while the worker writes it.

Commit `823c1a9` adds release and acquire publication. Device creation now waits for immutable configuration data before its first read.

The loader callback does not wait for this data. As a result, configuration loading cannot block the loader path.

The polling path remains available if the early import hook is unavailable. The change does not create a Direct3D probe device.

The review also found three display-profile defects. The follow-up corrected 640x480, renamed 1150x864 to 1152x864, and added 1280x720.

New tests contain fixed expected values for these three profiles. The Win32 Release build completed, and CTest passed 1 of 1 tests.

The configurator review found unsafe reads and a dead end for a Bin without an INI file. It also found stale values after a directory switch.

Configurator commit `f43234e` contains the follow-up. It loads bundled defaults, creates missing files, and reports read errors.

The same commit clears stale values, preserves save errors, corrects initial DPI units, and prevents duplicate folder pickers. It also rejects `NaN` values.

Direct tests now cover missing, locked, and newly created INI files. The configurator suite passed 39 of 39 tests.

The single-file publish did not copy the first external defaults file. Commit `bc0e65d` embeds this file into the configurator executable.

The original pull requests merged before this second review completed. New pull requests #3 and #4 now contain the follow-up corrections.

Pull request #3 contains the Direct3D and display-profile corrections. Pull request #4 contains the configuration-file and packaging corrections.

The pull-request text identifies the frame patch as value validation at a known RVA. It identifies menu and Bink behavior as observed results.

## 2026-08-12: Experimental source checkpoint

The deployed test binary used an uncommitted keyboard-prompt worktree. Native and configurator tests passed before the checkpoint.

Commit `16188e0` now preserves this source on `feature/dynamic-keyboard-prompts`. The local build directory remains untracked.

## Current state

The first runtime and configurator requests are merged. The automated review services must now examine follow-up requests #3 and #4.

The keyboard renderer works for the tested live remap. It needs broader action coverage before a separate pull request.

Automatic input selection remains incomplete. Startup stability and focus behavior also need longer tests.
