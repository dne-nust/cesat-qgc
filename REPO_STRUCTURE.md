# cesat-qgc — Repository Structure Reference

For a complete map of all files — including what engineers must create and when — see [REPO_STRUCTURE.md](REPO_STRUCTURE.md)

## Overview

`cesat-qgc` is the CESAT/NUST fork of QGroundControl (QGC). It adds four CESAT-specific monitoring panels and a custom parameter editor to the stock QGC interface, implemented as purely additive overlay widgets that activate only when CESAT MAVLink messages (60001–60004) are detected. QGC never contains SPI bus numbers, FPGA register addresses, or sensor-path logic — those details exist only in PX4 firmware and surface exclusively as CESAT MAVLink messages from cesat-px4autopilot. This separation means QGC and firmware can be developed, tested, and updated independently. The repository is opened with Qt 6.6.x (Qt Creator or CMake + Ninja on Ubuntu 22.04 or Windows MSVC 2022). It fits into the 5-board CESAT architecture as the ground station layer: it communicates with the STM32H753 FMUM (Board 1) exclusively over MAVLink via a telemetry radio or USB connection, displaying sensor health, FPGA status, algorithm state, and providing algorithm parameter control.

## How to Read This Document

| Symbol | Meaning |
|--------|---------|
| ✅ EXISTS | File was created at repo setup with full correct content. Do not modify without a formal change request referencing a document version bump (SDP v1.5). |
| 🔧 SKELETON | File was created at repo setup with placeholder structure only. The QGC engineer must implement all sections marked `TODO` before the specified milestone. |
| 📋 CREATE | File does not yet exist. The responsible engineer must create it before the specified milestone gate. |

---

## Repository Structure

### Root

```
cesat-qgc/
```

`.gitignore`
  Status: ✅ EXISTS
  Description: Standard QGC .gitignore extended for CESAT build outputs and generated MAVLink C headers.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Generated dialect headers (mavlink/cesat_dialect/generated/) should be in .gitignore if CI regenerates them fresh; or committed if QGC requires them at build time without Python available.

`.gitmodules`
  Status: ✅ EXISTS
  Description: Declares `mavlink/cesat_dialect` as a git submodule pointing to the dialect directory in cesat-px4autopilot. Both repos reference the same source dialect XML.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: When cesat.xml changes in cesat-px4autopilot, update the submodule pointer in cesat-qgc simultaneously. The dialect-sync CI workflow (`.github/workflows/dialect-sync.yml`) validates this.

`LICENSE`
  Status: ✅ EXISTS
  Description: GNU GPL v3, matching upstream QGC licence.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`README.md`
  Status: ✅ EXISTS
  Description: QGC fork overview, CESAT panels list, build instructions summary, and link to REPO_STRUCTURE.md.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`CONTRIBUTING.md`
  Status: ✅ EXISTS
  Description: Fork maintenance policy: minimise diff from upstream QGC (all CESAT additions go in `src/CesatUI/` and `custom/`), upstream cherry-pick cadence (quarterly), and PR requirements (must include dialect sync check).
  Responsible: PI
  Milestone: Created at repo setup
  Notes: SR07 risk: divergence from upstream QGC accumulates over time. Contributor must check if upstream QGC has addressed the changed area before modifying any non-CESAT file.

---

### .github/

`.github/pull_request_template.md`
  Status: ✅ EXISTS
  Description: PR checklist: dialect sync check (does cesat.xml submodule pointer match cesat-px4autopilot?), build pass on Linux and Windows, no modifications to upstream QGC files outside `src/CesatUI/` and `custom/`, OI reference if applicable.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`.github/ISSUE_TEMPLATE/build_issue.md`
  Status: ✅ EXISTS
  Description: Issue template for build failures — captures OS, Qt version, CMake version, and full error output.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`.github/ISSUE_TEMPLATE/mavlink_issue.md`
  Status: ✅ EXISTS
  Description: Issue template for MAVLink message handling issues — captures message ID (60001–60004), field name, expected vs. observed value, and PX4 firmware version.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`.github/ISSUE_TEMPLATE/ui_bug.md`
  Status: ✅ EXISTS
  Description: Issue template for UI bugs — captures panel name, QML file, screenshot, and repro steps.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: —

`.github/workflows/build-linux.yml`
  Status: ✅ EXISTS
  Description: CI workflow — builds QGC Linux AppImage on Ubuntu 22.04 on every push to main. Uploads AppImage as build artifact. Smoke-tests that QGC launches without crash.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Qt 6.6.x must be pinned in this workflow — do not auto-upgrade Qt without testing CESAT panels (SR07 risk).

`.github/workflows/build-windows.yml`
  Status: ✅ EXISTS
  Description: CI workflow — builds QGC Windows NSIS installer on Windows Server with MSVC 2022 on every push to main. Uploads installer as build artifact.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Windows build takes significantly longer than Linux — consider caching Qt installation.

`.github/workflows/dialect-sync.yml`
  Status: ✅ EXISTS
  Description: CI workflow — validates that the cesat_dialect submodule pointer matches the current head of cesat-px4autopilot/mavlink/cesat_dialect/. Runs `python -m pymavlink.tools.mavgen` to regenerate C headers and verifies no diff. Fails PR if submodule is stale.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: This prevents a class of bugs where QGC is compiled against an older dialect than the running firmware, causing field misalignment.

---

### custom/ — QGC Custom Build Integration (Task G, M2)

`custom/custom.cmake`
  Status: 🔧 SKELETON
  Description: QGC custom CMake hook. Adds `src/CesatUI/` subdirectory to the build and sets include path for generated CESAT MAVLink C headers. Dialect generation step (Python `mavgen` invocation) present as commented TODO — must be activated at M2 so CI generates fresh headers from cesat.xml without requiring manual regeneration.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: The `add_custom_command` for dialect generation must be added before M2 CI pipeline is declared ready. Without it, cesat.h must be committed manually — fragile and error-prone.

`custom/CustomFlyView.qml`
  Status: 📋 CREATE
  Description: QML override for QGC FlyView. Extends the upstream FlyView to embed CESAT overlay widgets: FPGA health indicator badge (top-right corner, active when CESAT_FPGA_STATUS msg 60001 is received), active sensor path badge (A/B/C), and algorithm mode indicator. Must degrade gracefully if CESAT messages are absent — show nothing rather than errors.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: Keep overlay minimal at M2 — full algorithm switching UI (Task K) is M4. Only passive status display at M2.

`custom/CustomQuickInterface.qml`
  Status: 📋 CREATE
  Description: QML file for CESAT quick-access interface. Adds CESAT status section to the QGC toolbar. Provides quick-view badges for FPGA health (60001) and active sensor path (60002). Links to full CESAT panel drawer.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: Must not break upstream QGC quick interface if CESAT messages are absent. Use `Loader` with `active: cesatHandler.fpgaOk` pattern.

---

### src/CesatUI/ — CESAT UI Module

`src/CesatUI/CesatMavlinkHandler.h`
  Status: ✅ EXISTS
  Description: C++ QObject header exposing CESAT MAVLink data to QML via Q_PROPERTY bindings. M2 properties: fpgaVersion, uartCount, fpgaOk, sensorPath, carrierDetect (from msg 60001); imu1Ok, imu2Ok, imu3Ok, precImuOk, stim300Ok, baro1Ok, baro2Ok (from msg 60002). M3 handlers (CESAT_ALGORITHM_STATUS 60003, CESAT_ESTIMATOR_STATE 60004) present as TODO. Signals: fpgaStatusChanged, imuStatusChanged.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Cross-sync required: any field added to cesat.xml (60001–60004) must be added as a Q_PROPERTY here and handled in CesatMavlinkHandler.cpp. Simultaneous PR to cesat-px4autopilot and cesat-qgc required for dialect changes.

`src/CesatUI/CMakeLists.txt`
  Status: 🔧 SKELETON
  Description: CMake source list for the CesatUI module. Declares CesatMavlinkHandler.cpp (TODO) and .h as build sources. QML files for all panels are present as commented-out TODO entries: cesat_fpga_status.qml (M2), cesat_imu_status.qml (M2), cesat_algorithm_status.qml (M3), cesat_estimator_state.qml (M3). Engineer must uncomment and add new QML files as they are created.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: Qt resource system requires QML files to be declared here as Qt resources before they can be loaded at runtime.

`src/CesatUI/CesatMavlinkHandler.cpp`
  Status: 📋 CREATE
  Description: Implementation of CesatMavlinkHandler.h. Must implement handleCesatFpgaStatus() parsing mavlink_cesat_fpga_status_t fields (fpga_version, uart_count, fpga_ok, sensor_path, carrier_detect) and emitting fpgaStatusChanged(); handleCesatImuStatus() parsing mavlink_cesat_imu_status_t (imu1_ok through baro2_ok) and emitting imuStatusChanged(). M3: add handleCesatAlgorithmStatus() and handleCesatEstimatorState(). Must be registered with QGC MAVLink message dispatcher in QGC's MAVLinkProtocol message handler.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: All handleCesat*() methods must guard against missing message IDs — return immediately without error if message ID is not 60001–60004. This ensures graceful operation with non-CESAT firmware.

---

### src/CesatUI/ — QML Panels (Task H, M2/M3)

`src/CesatUI/cesat_fpga_status.qml`
  Status: 📋 CREATE
  Description: QML panel for CESAT FPGA Status (MAVLink message 60001). Displays: FPGA firmware version (major.minor from fpgaVersion Q_PROPERTY), UART core count (uartCount, green if =15, red otherwise), FPGA health indicator (fpgaOk: green tick / red cross), active sensor path (sensorPath: 1=A MEMS / 2=B Precision / 3=C FOG with text and colour), carrier detect ID (carrierDetect: 0=None / 1=ADIS16488A / 2=KT-EX9-1 / 3=KT-EX9-2). Panel must be visible only when fpgaOk has been received at least once (use `visible: cesatHandler.fpgaVersion > 0`).
  Responsible: QGC Engineer
  Milestone: M2
  Notes: SDP v1.5 Section 5.2 (Task H, FPGA Health panel). Data source: CESAT_FPGA_STATUS (60001) via CesatMavlinkHandler. Do not hardcode bus numbers or register addresses in QML — read only from Q_PROPERTY values.

`src/CesatUI/cesat_imu_status.qml`
  Status: 📋 CREATE
  Description: QML panel for CESAT IMU Status (MAVLink message 60002). Displays: per-IMU health rows — BMI088 IMU1 (imu1Ok), ICM-42688-P IMU2 (imu2Ok), ICM-45686 IMU3 (imu3Ok); Precision sensor Path B (precImuOk: green/red/grey when absent); STIM300 Path C (stim300Ok: green/red/grey when absent); BMP581 BARO1 I2C4 (baro1Ok); BMP581 BARO2 I2C2 (baro2Ok). All health indicators use green/red for present+healthy/fault; grey for absent/not-fitted.
  Responsible: QGC Engineer
  Milestone: M2
  Notes: SDP v1.5 Section 5.2 (Task H, Precision IMU panel). STIM300 will show grey until Path C is activated at M3 (OI-06). Panel must not alarm on grey — grey means sensor not fitted, not fault.

`src/CesatUI/cesat_algorithm_status.qml`
  Status: 📋 CREATE
  Description: QML panel for CESAT Algorithm Status (MAVLink message 60003). Displays: active estimator type (estimator_type enum decoded per OI-08: 0=none, 1=EKF, 2=UKF, 3=complementary), convergence indicator (converged), 1-sigma position/velocity/attitude uncertainty bars, watchdog fallback count (if non-zero, show warning banner). Algorithm switching confirmation modal is separate (see CesatParameterEditor).
  Responsible: QGC Engineer
  Milestone: M3
  Notes: SDP v1.5 Section 5.2 (Task H, Estimator Status panel). Data source: CESAT_ALGORITHM_STATUS (60003). OI-08 must be resolved before estimator_type enum values are final — display as raw integer until OI-08 is resolved if needed.

`src/CesatUI/cesat_estimator_state.qml`
  Status: 📋 CREATE
  Description: QML panel for CESAT Estimator State (MAVLink message 60004). Displays: attitude quaternion decoded to roll/pitch/yaw Euler angles, NED position (pn/pe/pd in metres), NED velocity (vn/ve/vd in m/s). All values displayed as numerical readouts with units. Panel active only when CESAT estimator is running (check cesat_algorithm_status converged = 1 before displaying).
  Responsible: QGC Engineer
  Milestone: M3
  Notes: Message 60004 is sent at 50 Hz when CESAT estimator is active — QML binding updates must not cause UI jank. Use a 10 Hz display update timer; bind to Q_PROPERTY but throttle redraws.

---

### src/CesatUI/ — Parameter Editor (Task I, M3)

`src/CesatUI/CesatParameterEditor.h`
  Status: 📋 CREATE
  Description: C++ QObject header for the CESAT parameter editor. Exposes Q_PROPERTY for CESAT_EST_EN (estimator enable) and CESAT_CTRL_EN (controller enable) parameter values. Provides Q_INVOKABLE methods: setEstimatorEnabled(bool), setControllerEnabled(bool) — both trigger MAVLink PARAM_SET; readAllCesatParams() — reads all CESAT_ prefixed parameters via MAVLink PARAM_REQUEST_READ; savePreset(name), loadPreset(name) — JSON file operations for parameter preset management.
  Responsible: QGC Engineer
  Milestone: M3
  Notes: SDP v1.5 Section 5.3 (Task I). Safety confirmation modal (requiring user to type "CONFIRM" before enabling custom stack) must gate setEstimatorEnabled(true) — never allow silent activation of custom estimator.

`src/CesatUI/CesatParameterEditor.cpp`
  Status: 📋 CREATE
  Description: Implementation of CesatParameterEditor.h. QGC engineer deliverable.
  Responsible: QGC Engineer
  Milestone: M3
  Notes: Use QGC's existing ParameterManager infrastructure for MAVLink PARAM_SET/GET — do not reimplement MAVLink parameter protocol.

`src/CesatUI/cesat_parameter_editor.qml`
  Status: 📋 CREATE
  Description: QML parameter editor panel with three groups: Algorithm Enable (CESAT_EST_EN and CESAT_CTRL_EN toggle switches with safety confirmation modal — modal requires explicit "CONFIRM" button press, not just a checkbox, before switching to custom stack), Algorithm Tuning (CESAT estimator and controller gain parameters rendered as sliders and numerical inputs with units), Preset Management (save/load named JSON presets with filename input and preset list).
  Responsible: QGC Engineer
  Milestone: M3
  Notes: SDP v1.5 Section 5.3 (Task I). Safety modal is not optional — SR05 risk: cesat_estimator divergence without watchdog is a safety-critical failure. The 50ms watchdog in cesat_diagnostics is the firmware backstop, but the confirmation dialog is the operator safeguard.

---

### src/CesatUI/ — UART Channel Table & Protocol Monitor (Task J, M3/M4)

`src/CesatUI/cesat_uart_channel_table.qml`
  Status: 📋 CREATE
  Description: QML panel displaying FPGA UART channel table (UART0–13 plus UART14 STIM300). For each channel: channel number, baud rate (from cesat_fpga_status Q_PROPERTY if exposed, or static table), protocol type (RS422/RS485/TTL/STIM300), RX FIFO fill level (from UART_FIFO_FILL field in 60001 if added), status (active/idle/fault). Raw frame monitor view: selectable channel, hex dump of last N received bytes.
  Responsible: QGC Engineer
  Milestone: M3 (table + raw monitor); M4 (protocol decode views after OI-09 resolved)
  Notes: SDP v1.5 Section 5.4 (Task J). OI-09 must be resolved before protocol decode views can be implemented — without protocol definition, only raw hex display is possible. CESAT_FPGA_STATUS message (60001) may need additional UART FIFO fill fields to support live FIFO monitoring — coordinate with cesat-px4autopilot team if field additions are needed.

---

### src/CesatUI/ — Flight Mode UI (Task K, M4)

`src/CesatUI/cesat_flight_mode.qml`
  Status: 📋 CREATE
  Description: QML flight mode UI for the QGC FlyView. Adds CESAT Estimator and Controller ON/OFF buttons in the fly view toolbar. Fallback alert banner: displayed prominently (red banner, audio alert) when watchdog fallback occurs (detected via cesat_algorithm_status watchdog_state field or via MAVLink STATUSTEXT from cesat_diagnostics). HUD algorithm status overlay: small text overlay on HUD showing "EST: CESAT" or "EST: EKF2" and "CTRL: CESAT" or "CTRL: PX4".
  Responsible: QGC Engineer
  Milestone: M4
  Notes: SDP v1.5 Section 5.4 (Task K). ON/OFF buttons must trigger the same safety confirmation modal as CesatParameterEditor (do not allow silent in-flight mode switching without operator acknowledgement).

---

### src/CesatUI/ — ROS2/uXRCE-DDS Panel (Task M, M4)

`src/CesatUI/cesat_ros2_panel.qml`
  Status: 📋 CREATE
  Description: QML panel for ROS2/uXRCE-DDS integration status. Displays: uXRCE-DDS agent connection status (on-FMU vs. off-FMU deployment), active DDS topics list, bridge bandwidth estimate. Depends on Task D3 (uXRCE-DDS bridge in cesat-px4autopilot). Scope may be adjusted based on OI-08 algorithm deployment decision.
  Responsible: QGC Engineer
  Milestone: M4
  Notes: SDP v1.5 Section 5.4 (Task M). Requires Task D3 to be functional in cesat-px4autopilot before meaningful data is available. Panel must degrade gracefully if uXRCE-DDS is not running.

---

### mavlink/ — CESAT Dialect Submodule

`mavlink/cesat_dialect/`
  Status: ✅ EXISTS
  Description: Git submodule pointing to cesat-px4autopilot/mavlink/cesat_dialect/. Contains cesat.xml (4 CESAT messages: 60001–60004) and README. Managed by .gitmodules entry.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Update submodule pointer whenever cesat.xml changes in cesat-px4autopilot. dialect-sync.yml CI workflow enforces this.

`mavlink/cesat_dialect/generated/c/cesat.h`
  Status: 📋 CREATE
  Description: Auto-generated C MAVLink header from cesat.xml. Generated by `python -m pymavlink.tools.mavgen --lang C --wire-protocol 2.0 cesat.xml`. Consumed by CesatMavlinkHandler.cpp for message decoding (mavlink_cesat_fpga_status_t, mavlink_cesat_imu_status_t, etc.). Must not be hand-edited.
  Responsible: CI (auto-generated via custom.cmake)
  Milestone: M2
  Notes: Once custom.cmake TODO is activated, this file is generated fresh at every build. Until then it must be committed manually. Add to .gitignore only after custom.cmake generation is active.

---

### docs/ — QGC Development Documentation

`docs/build_instructions.md`
  Status: ✅ EXISTS
  Description: Complete build instructions for Linux (Ubuntu 22.04, Qt 6.6.x apt packages) and Windows (Qt 6.6.x installer, MSVC 2022). Covers CMake build steps, CESAT custom build flag (`-DCESAT_QGC=ON`), and AppImage packaging on Linux.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Pin Qt version explicitly. Do not upgrade Qt without PI review — SR07 risk of Qt API incompatibility.

`docs/cesat_panels.md`
  Status: ✅ EXISTS
  Description: Specification for all four CESAT panels: Panel 1 FPGA Status (msg 60001, M2), Panel 2 IMU Status (msg 60002, M2), Panel 3 Algorithm Status (msg 60003, M3), Panel 4 Estimator State (msg 60004, M3). Each entry lists QML source file path, data source MAVLink message ID, and UI element list.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Update this document whenever a panel's field list changes due to cesat.xml updates. cesat_panels.md is the QGC engineer's primary specification document for UI implementation.

`docs/mavlink_messages.md`
  Status: ✅ EXISTS
  Description: Human-readable description of CESAT MAVLink messages 60001–60004. Mirrors cesat.xml field definitions. Includes field names, types, units, and update rates. This document is the cross-team reference — cesat-px4autopilot engineers publish to these messages; cesat-qgc engineers consume from them.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Cross-sync required with cesat.xml in cesat-px4autopilot and CesatMavlinkHandler.h. Any field addition requires simultaneous update to all three. Do not modify docs/mavlink_messages.md without a corresponding cesat.xml PR.

`docs/algorithm_switching.md`
  Status: ✅ EXISTS
  Description: Design specification for the algorithm switching UI workflow (Task I). Documents the safety confirmation modal requirements, the CESAT_EST_EN / CESAT_CTRL_EN parameter flow via MAVLink PARAM_SET, the expected state transitions (EKF2-only → CESAT estimator active → watchdog fallback → EKF2-only), and what the QGC UI must display at each state.
  Responsible: PI
  Milestone: Created at repo setup
  Notes: Cross-sync required with SDP v1.5 Section 5.3 (Task I). If the CesatEstimatorBase interface changes (cesat-px4autopilot), update this document to reflect the new parameter/state model.

---

## Priority Actions for New Engineers

1. **Activate the dialect generation step in custom/custom.cmake before M2 CI pipeline is declared ready.** The `add_custom_command` for `pymavlink.tools.mavgen` is present as a TODO comment. Without it, generated C headers must be committed manually — any cesat.xml change will silently leave stale headers in the build, causing field offset mismatches at runtime.

2. **Implement CesatMavlinkHandler.cpp (M2) before any QML panels are testable.** The QML panels bind to Q_PROPERTY values in CesatMavlinkHandler — without the .cpp implementation, all panel fields show default (zero/false) values even when connected to real hardware. This is the first M2 task to start.

3. **Register CesatMavlinkHandler with QGC's MAVLink dispatcher.** CesatMavlinkHandler.cpp must be wired into QGC's MAVLinkProtocol message handling pipeline. Without this registration step, handleCesatFpgaStatus() and handleCesatImuStatus() are never called regardless of incoming MAVLink traffic. Consult QGC contributing docs for the correct hook point.

4. **Uncomment QML file entries in src/CesatUI/CMakeLists.txt as each panel file is created.** The QML files are commented out as TODO entries. Each file must be added as a Qt resource before it can be loaded at runtime — forgetting this step produces a silent failure (panel loads blank).

5. **Resolve OI-08 before implementing cesat_algorithm_status.qml (M3).** The estimator_type enum in CESAT_ALGORITHM_STATUS (60003) depends on the algorithm type decided in OI-08. Until OI-08 is resolved, the panel can display the raw integer but cannot show a human-readable algorithm name.

6. **Resolve OI-09 before implementing protocol decode views in cesat_uart_channel_table.qml (M4).** The raw frame monitor can be built at M3 without OI-09. Protocol-aware decode views (framing, CRC, channel multiplexing) require the protocol definition from OI-09.

7. **Pin Qt version in CI workflows before the first M2 build.** Qt 6.6.x must be pinned in build-linux.yml and build-windows.yml. Auto-upgrading Qt without testing is the primary cause of cesat-qgc fork drift (SR07 risk).

---

## Cross-Repository Dependencies

| File in cesat-qgc | Synced with | Sync requirement |
|-------------------|-------------|-----------------|
| `mavlink/cesat_dialect/` (submodule) | `cesat-px4autopilot/mavlink/cesat_dialect/cesat.xml` | Submodule pointer must point to same commit as cesat-px4autopilot dialect. dialect-sync.yml enforces this. |
| `src/CesatUI/CesatMavlinkHandler.h` (Q_PROPERTY field list) | `cesat-px4autopilot/mavlink/cesat_dialect/cesat.xml` (message fields) | Every cesat.xml field must have a corresponding Q_PROPERTY. Simultaneous PR to both repos required for field changes. |
| `docs/mavlink_messages.md` | `cesat-px4autopilot/mavlink/cesat_dialect/cesat.xml` | Must reflect current cesat.xml exactly. Any cesat.xml change triggers a mavlink_messages.md update PR. |
| `docs/algorithm_switching.md` | `cesat-px4autopilot/src/lib/cesat/algorithm_base/CesatEstimatorBase.hpp` | Algorithm enable/disable flow in QGC must match the CESAT_EST_EN / CESAT_CTRL_EN parameter contract in PX4. Any interface change requires coordinated update. |
| `src/CesatUI/cesat_algorithm_status.qml` (estimator_type enum decode) | `cesat-px4autopilot/src/modules/cesat_estimator/module.yaml` (CESAT_EST_EN enum values) | Enum values in QML decode table must match parameter definitions in PX4 module.yaml. OI-08 resolution fixes both simultaneously. |
| `mavlink/cesat_dialect/generated/c/cesat.h` | `cesat-px4autopilot/mavlink/cesat_dialect/cesat.xml` | Auto-generated — must be regenerated whenever cesat.xml changes. Never manually edited. |

---

## Naming Conventions

**C++ handler files:** `Cesat<Name>.h` / `Cesat<Name>.cpp` in `src/CesatUI/` — e.g., `CesatMavlinkHandler.h`, `CesatParameterEditor.h`. PascalCase class names prefixed with `Cesat`.

**QML panel files:** `cesat_<panel_snake_case>.qml` in `src/CesatUI/` — e.g., `cesat_fpga_status.qml`, `cesat_algorithm_status.qml`. Snake_case, all lowercase.

**Custom QML overrides:** `Custom<Name>.qml` in `custom/` — e.g., `CustomFlyView.qml`, `CustomQuickInterface.qml`. Matches QGC custom build hook naming convention.

**CMake files:** `custom.cmake` in `custom/` (QGC hook point, fixed name). `CMakeLists.txt` in each `src/` subdirectory.

**Documentation files:** `<snake_case_topic>.md` in `docs/` — e.g., `cesat_panels.md`, `mavlink_messages.md`, `algorithm_switching.md`.

**Task mapping (SDP v1.5 → files):**
- Task G (QGC fork): `custom/CustomFlyView.qml`, `custom/CustomQuickInterface.qml`, `custom/custom.cmake`
- Task H (telemetry display): `cesat_fpga_status.qml`, `cesat_imu_status.qml`, `cesat_algorithm_status.qml`, `cesat_estimator_state.qml`
- Task I (parameter editor): `CesatParameterEditor.h/.cpp`, `cesat_parameter_editor.qml`
- Task J (protocol monitor): `cesat_uart_channel_table.qml`
- Task K (flight mode UI): `cesat_flight_mode.qml`
- Task M (ROS2 panel): `cesat_ros2_panel.qml`
