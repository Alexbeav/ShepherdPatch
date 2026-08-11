# ShepherdPatch Project Report

Date: 2026-08-12

Project state: Active development

Target: Silent Hill Homecoming for Windows

## Executive summary

This project changed ShepherdPatch from a difficult local patch into a practical modernization package.

The work corrected three major user problems:

- The configurator now supports Steam libraries on any drive.
- The runtime now produces 60 FPS gameplay instead of changing only loading screens.
- High-resolution maps, documents, and item panels now use the correct canvas size.

The project also produced a keyboard-prompt prototype. This prototype reads live bindings and updates prompts after an in-game remap.

Two focused upstream pull requests delivered the first stable work. The maintainer merged both requests.

A second review found six important defects across both requests. Follow-up pull requests #3 and #4 contain the corrections and regression tests.

## Project goals

The project has these goals:

1. Make ShepherdPatch work outside the default `C:` Steam library.
2. Give the configurator a clear basic view and a complete advanced view.
3. Make the frame-rate option affect gameplay.
4. Preserve normal playback speed for prerendered movies.
5. Correct high-resolution panels without damaging text or menus.
6. Replace generic keyboard prompts with labels from active bindings.
7. Preserve original difficulty, QTE behavior, and game rules.

## Project boundaries

ShepherdPatch and UnknownProject remain separate projects.

No UnknownProject source, binary, or asset was copied into ShepherdPatch. The UnknownProject review supplied comparison data only.

The Full Stereo Remaster also remains separate. Its reported compatibility depends on the installed UnknownProject version and audio-device configuration.

## Repository comparison

| Area | ShepherdPatch | UnknownProject 3.20 |
| --- | --- | --- |
| Loader | Local `version.dll` proxy and `ShepherdPatch.asi` | Modified game binaries and `shv.dll` |
| Original binaries | Preserved | Replaced |
| Frame rate | Runtime hooks and engine interval patch | Modified engine code |
| Display | Borderless mode, DPI, FOV, HUD, and panel corrections | Resolution and widescreen engine changes |
| Input | Raw mouse, DirectInput recovery, and prompt prototype | Mouse, controller, and prompt changes |
| Gameplay | Original behavior is the target | QTE, health, difficulty, save, and objective changes |
| Configuration | Named values in `ShepherdPatch.ini` | Position-sensitive values in `shv.cfg` |
| License | MIT | No repository license was found during the review |

## Stable runtime work

Runtime pull requests: [#1](https://github.com/xowny/ShepherdPatch/pull/1) and [#3](https://github.com/xowny/ShepherdPatch/pull/3)

The runtime work includes these changes:

- It patches the stock engine frame interval after a value validation at a known RVA.
- It intercepts the engine load before `LoadLibraryA` returns control to the game.
- It installs the Direct3D creation hook before the engine can create the real device.
- It retains module polling as a fallback without creating a probe device.
- The tested menu remained at 30 FPS while gameplay used the selected frame rate.
- The tested prerendered Bink movies played at their intended speed.
- It selects high-resolution panel constants from both backbuffer dimensions.
- It preserves established item scales at intermediate resolutions, including 1600x900.
- It has explicit profiles for 640x480, 1152x864, and 1280x720.
- It makes the high-resolution patch an all-or-nothing operation.
- It hardens Direct3D device creation, retries, resets, and startup ordering.
- It records useful diagnostics in one persistent, flushed log stream.
- It preserves requested delays in the legacy waitable-timer policy.

The desktop test validated 60 FPS gameplay. The menu remained at 30 FPS, and loading screens reached 60 FPS.

The same test validated normal movie speed after the Bink separation. Earlier builds played prerendered movies at double speed.

## Stable configurator work

Configurator pull requests: [#2](https://github.com/xowny/ShepherdPatch/pull/2) and [#4](https://github.com/xowny/ShepherdPatch/pull/4)

The configurator work includes these changes:

- It replaces the fixed Steam path with a **Choose game Bin** command.
- It requires `SilentHill.exe` before it accepts a selected directory.
- It stores the selected directory under `%LocalAppData%\ShepherdPatch`.
- It protects unsaved changes during a directory switch.
- It shows a small basic view by default.
- It places less common controls behind **Advanced options**.
- It uses a responsive card layout and a compact footer layout.
- It keeps **Discard**, **Save**, and **Run** visible at practical window sizes.
- It offers optional Steam launch without changing the default direct launch.
- It reports configuration-write and launch errors to the user.
- It reports configuration-read errors without terminating the application.
- It loads bundled defaults when a selected Bin has no configuration file.
- It creates the missing configuration file when the user selects **Save**.
- It clears values from the previous installation during a directory switch.
- It uses invariant decimal formatting for all Windows locales.
- It replaces cleared numeric fields with documented fallback values.

The configurator no longer creates or opens a guessed game directory. This change removes the original crash path.

## Keyboard-prompt prototype

The stock game already stores command tokens and keyboard bindings. Its PC prompt assets do not include images for each keyboard key.

The prototype replaces command tokens with text labels. Examples include `[LMB] Open` and `[Tab] Inventory`.

The prototype also reads the live binding table. A tested Inventory remap changed the visible prompt without a game restart.

The prototype is not part of either upstream pull request. It still needs a complete action matrix and a focused pull request.

Known prompt gaps include generic controller numbers and some uncommon action labels. The work must retain the stock prompt when resolution fails.

## Dynamic input experiment

Automatic keyboard and controller selection remains experimental.

The current experiment detects some XInput activity and changes the engine input mode. It does not restore all controller controls.

The following controller actions worked during the test:

- `Y` opened the map.
- `L1` and `R1` opened selection wheels.
- `X` and `Y` controlled the flashlight and radio in the inventory.
- Vertical right-stick movement worked.

The left stick, triggers, and most other controls did not work. This result indicates that one mode byte is not sufficient.

## Launch and focus investigation

Early builds showed intermittent startup hangs before the logos. Some runs also stopped at the main menu.

Device startup and retry changes improved this behavior. Four sequential launches worked during one desktop test, including two Steam launches.

The laptop build also loaded a save. Its first load stopped near completion, and its second load worked.

These results do not prove that startup is fully stable. A longer launch and shutdown test remains necessary.

The game also has fragile focus behavior. After the main menu loads, background rendering can stop until the window regains focus.

## Validation summary

| Area | Result | Evidence |
| --- | --- | --- |
| Native runtime build | Pass | Win32 Release build completed |
| Native tests | Pass | CTest completed with 1 of 1 tests passed |
| Configurator tests | Pass | 39 of 39 tests passed |
| Configurator publish | Pass | Self-contained `win-x64` publish completed |
| Gameplay frame rate | Pass at 60 FPS | Desktop play test |
| Menu frame rate | Expected 30 FPS | Desktop play test |
| Prerendered movies | Normal speed | Desktop play test |
| High-resolution panels | Pass | Map, note, and item screenshots |
| Live keyboard remap | Pass for tested Inventory action | In-game remap test |
| Laptop package | Partial pass | Game and save loaded after one failed load |
| Dynamic controller mode | Incomplete | Only a subset of controls worked |
| 144 FPS | Not tested | Deferred test |

## Pull-request status

| Pull request | Head commit | State |
| --- | --- | --- |
| Runtime #1 | `9ac3c6f` | Merged |
| Configurator #2 | `32912c6` | Merged |
| Runtime follow-up #3 | `bcc6693` | Open and mergeable, Cubic passed |
| Configurator follow-up #4 | `e448e2d` | Open and mergeable, Cubic passed |

Automated review found several real problems. Earlier commits corrected memory protection, retry parameters, logging, layout, and error handling.

The second review found a late Direct3D hook window and three incorrect display profiles. Runtime commit `0720ab2` corrects these defects.

A follow-up review found an early configuration-publication race. Runtime commit `823c1a9` publishes immutable configuration data before the first device hook reads it.

Cubic then found an unbounded wait in this gate. Runtime commit `bcc6693` adds an event, a five-second limit, logging, and a stock-call fallback.

The same review found unsafe configuration reads and an unusable fresh-install path. Configurator commits `f43234e` and `bc0e65d` correct these defects.

Cubic found three follow-up defects in the configurator. Commit `e448e2d` corrects initial DPI timing, inaccessible-file detection, and picker error scope.

The frame interval uses value validation at a known data RVA. It does not use a signature at that data site.

The menu and Bink results are observations from game tests. The runtime does not enforce separate menu or movie frame-rate limits.

## Known risks

- Startup stability needs a longer repeated-launch test.
- Focus changes can stop visual updates after the main menu loads.
- Keyboard-prompt coverage is incomplete.
- Generic gamepad prompt numbers remain unresolved.
- Automatic input selection does not restore the full controller state.
- Frame rates above 60 FPS have no gameplay validation.
- Fixed engine addresses and absolute operands target the known retail image layout.
- ShepherdPatch and UnknownProject still modify several of the same runtime paths.

## Recommended next work

1. Ask the maintainer to merge pull requests #3 and #4.
2. Build an action matrix for all keyboard prompts.
3. Submit the keyboard renderer as a separate native pull request.
4. Record the full controller state before and after a manual input-mode change.
5. Replace the single-byte controller experiment with the validated state transition.
6. Run at least 50 launch, load, exit, and relaunch cycles.
7. Test 120 FPS and 144 FPS with movies, combat, physics, and QTE sequences.
8. Publish a preview package after the laptop validation is complete.

## Build commands

Use the Win32 platform for the native patch:

```powershell
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Release
ctest --test-dir build-win32 -C Release --output-on-failure
```

Use the Release configuration for the configurator:

```powershell
dotnet test gui\ShepherdConfigurator.Tests\ShepherdConfigurator.Tests.csproj -c Release
dotnet publish gui\ShepherdConfigurator\ShepherdConfigurator.csproj -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true
```

## Working branches

- `fix/runtime-rendering` contains the stable runtime pull request.
- `fix/configurator-usability` contains the stable configurator pull request.
- `feature/dynamic-keyboard-prompts` contains experimental prompt and input work.
- Commit `16188e0` preserves the tested experimental source on the fork.
- `contrib/combined-preview` contains the earlier combined preview package source.
