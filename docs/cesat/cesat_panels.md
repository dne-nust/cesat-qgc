# CESAT Custom Panels

This document describes the four CESAT-specific panels added to QGC.

## Panel 1 — CESAT FPGA Status (M2)

**Source:** `src/CesatUI/cesat_fpga_status.qml` (TODO: implement at M2)
**Data source:** MAVLink message 60001

Displays:
- FPGA firmware version (major.minor)
- UART core count (expect 15)
- FPGA health indicator (green/red)
- Active sensor path (A=MEMS / B=Precision / C=FOG)
- Carrier detect code (ADIS16488A / KT-EX9-1 / KT-EX9-2 / None)

## Panel 2 — CESAT IMU Status (M2)

**Source:** `src/CesatUI/cesat_imu_status.qml` (TODO: implement at M2)
**Data source:** MAVLink message 60002

Displays:
- BMI088 IMU1 (green/red)
- ICM-42688-P IMU2 (green/red)
- ICM-45686 IMU3 (green/red)
- Precision sensor — Path B (green/red/grey if absent)
- STIM300 — Path C (green/red/grey if absent)
- BMP581 BARO1 (green/red)
- BMP581 BARO2 (green/red)

## Panel 3 — CESAT Algorithm Status (M3)

**Source:** `src/CesatUI/cesat_algorithm_status.qml` (TODO: implement at M3)
**Data source:** MAVLink message 60003

Displays:
- Estimator type (EKF / UKF / Complementary / None)
- Convergence status
- Position, velocity, attitude uncertainty bars

## Panel 4 — CESAT Estimator State (M3)

**Source:** `src/CesatUI/cesat_estimator_state.qml` (TODO: implement at M3)
**Data source:** MAVLink message 60004

Displays full INS state vector (attitude quaternion + NED velocity + NED position) as numeric readouts alongside EKF2 for comparison.

## Algorithm Switching (M3)

See [docs/algorithm_switching.md](algorithm_switching.md) for the procedure to switch between EKF2 and the CESAT custom estimator.
