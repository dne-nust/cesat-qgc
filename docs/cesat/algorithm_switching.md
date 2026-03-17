# Algorithm Switching — EKF2 vs CESAT Custom INS

This page describes how to switch between PX4 EKF2 (standard) and the CESAT custom INS estimator (M3 feature).

## Availability

Algorithm switching requires:
- CESAT firmware M3 milestone or later
- `CESAT_EST_EN=1` parameter set on the vehicle
- OI-08 resolved (algorithm type selected by Dr. Najeeb Ahmad / Dr. Hamid Jabbar)

## Switching via QGC

1. Connect to CESAT FMUv6X
2. Navigate to Vehicle Setup → Parameters
3. Set `CESAT_EST_EN` to 1 (enable CESAT estimator)
4. Set `EKF2_EN` to 0 (disable PX4 EKF2) if running CESAT estimator exclusively
   - Note: Running both simultaneously is supported for comparison
5. Reboot the vehicle
6. Verify in CESAT Algorithm Status panel (MAVLink 60003): `converged = 1`

## Safety

Do not disable EKF2 in flight without verifying CESAT estimator convergence.
The CESAT estimator must output valid state estimates (CESAT_ESTIMATOR_STATE, 60004) before EKF2 is disabled.

## OI-08 Note

The CESAT estimator algorithm type is TBD (OI-08). Owner: Dr. Najeeb Ahmad + Dr. Hamid Jabbar. Target: Before M2 (30 Jun 2026).
