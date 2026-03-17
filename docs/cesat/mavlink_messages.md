# CESAT MAVLink Messages

QGC-side documentation for CESAT custom MAVLink messages. For XML definitions, see `mavlink/cesat_dialect/cesat.xml`.

## Message 60001 — CESAT_FPGA_STATUS

Sent by `cesat_diagnostics` module at `CESAT_DIAG_RATE` Hz (default 1 Hz).

| Field | Type | Description |
|-------|------|-------------|
| time_boot_ms | uint32 | ms since boot |
| fpga_version | uint16 | major<<8 \| minor |
| uart_count | uint8 | Expect 15 |
| fpga_ok | uint8 | 1=healthy |
| sensor_path | uint8 | 1=A, 2=B, 3=C |
| carrier_detect | uint8 | 0=none, 1=ADIS16488A, 2=KT-EX9-1, 3=KT-EX9-2 |

**QGC Panel:** CESAT FPGA Status. Shows version, UART count, sensor path selection.

## Message 60002 — CESAT_IMU_STATUS

Sent by `cesat_diagnostics` at `CESAT_DIAG_RATE` Hz.

| Field | Type | Description |
|-------|------|-------------|
| time_boot_ms | uint32 | ms since boot |
| imu1_ok | uint8 | BMI088 health |
| imu2_ok | uint8 | ICM-42688-P health |
| imu3_ok | uint8 | ICM-45686 health |
| prec_imu_ok | uint8 | Precision sensor health (Path B) |
| stim300_ok | uint8 | STIM300 health (Path C) |
| baro1_ok | uint8 | BMP581 BARO1 health |
| baro2_ok | uint8 | BMP581 BARO2 health |

**QGC Panel:** CESAT IMU Status. Shows green/red health indicators for each sensor.

## Message 60003 — CESAT_ALGORITHM_STATUS (M3)

Sent by `cesat_estimator` when `CESAT_EST_EN=1`.

| Field | Description |
|-------|-------------|
| estimator_type | 0=none, 1=EKF, 2=UKF, 3=complementary (OI-08 TBD) |
| converged | 1 when estimator converged |
| pos_uncertainty | 1-sigma position uncertainty (m) |
| vel_uncertainty | 1-sigma velocity uncertainty (m/s) |
| att_uncertainty | 1-sigma attitude uncertainty (rad) |

## Message 60004 — CESAT_ESTIMATOR_STATE (M3)

Full state vector from CESAT custom INS. Quaternion (w,x,y,z) + NED velocity + NED position.

## Graceful Degradation

If CESAT messages are absent (standard PX4 firmware connected), all CESAT panels must show a "Not Available — CESAT firmware not detected" placeholder. QGC must not crash or log errors when these messages are absent.
