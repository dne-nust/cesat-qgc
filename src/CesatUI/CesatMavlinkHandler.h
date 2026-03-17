/**
 * CesatMavlinkHandler.h — QGC handler for CESAT custom MAVLink messages
 *
 * Receives CESAT MAVLink messages (60001–60004) and exposes data to QML
 * panels via Q_PROPERTY bindings.
 *
 * Design principle: If CESAT messages are absent, all properties return
 * sensible defaults (0 / false). CESAT panels must degrade gracefully.
 *
 * Authority: SDP v1.5, Section 3.5 (MAVLink integration)
 */

#pragma once
#include <QObject>
#include <QString>

class CesatMavlinkHandler : public QObject {
    Q_OBJECT

    // CESAT_FPGA_STATUS (60001)
    Q_PROPERTY(int fpgaVersion READ fpgaVersion NOTIFY fpgaStatusChanged)
    Q_PROPERTY(int uartCount READ uartCount NOTIFY fpgaStatusChanged)
    Q_PROPERTY(bool fpgaOk READ fpgaOk NOTIFY fpgaStatusChanged)
    Q_PROPERTY(int sensorPath READ sensorPath NOTIFY fpgaStatusChanged)
    Q_PROPERTY(int carrierDetect READ carrierDetect NOTIFY fpgaStatusChanged)

    // CESAT_IMU_STATUS (60002)
    Q_PROPERTY(bool imu1Ok READ imu1Ok NOTIFY imuStatusChanged)
    Q_PROPERTY(bool imu2Ok READ imu2Ok NOTIFY imuStatusChanged)
    Q_PROPERTY(bool imu3Ok READ imu3Ok NOTIFY imuStatusChanged)
    Q_PROPERTY(bool precImuOk READ precImuOk NOTIFY imuStatusChanged)
    Q_PROPERTY(bool stim300Ok READ stim300Ok NOTIFY imuStatusChanged)
    Q_PROPERTY(bool baro1Ok READ baro1Ok NOTIFY imuStatusChanged)
    Q_PROPERTY(bool baro2Ok READ baro2Ok NOTIFY imuStatusChanged)

public:
    explicit CesatMavlinkHandler(QObject *parent = nullptr);

    // FPGA getters
    int fpgaVersion() const { return _fpgaVersion; }
    int uartCount() const { return _uartCount; }
    bool fpgaOk() const { return _fpgaOk; }
    int sensorPath() const { return _sensorPath; }
    int carrierDetect() const { return _carrierDetect; }

    // IMU getters
    bool imu1Ok() const { return _imu1Ok; }
    bool imu2Ok() const { return _imu2Ok; }
    bool imu3Ok() const { return _imu3Ok; }
    bool precImuOk() const { return _precImuOk; }
    bool stim300Ok() const { return _stim300Ok; }
    bool baro1Ok() const { return _baro1Ok; }
    bool baro2Ok() const { return _baro2Ok; }

    // Called by QGC MAVLink message dispatcher
    void handleCesatFpgaStatus(const void *msg);   // mavlink_cesat_fpga_status_t*
    void handleCesatImuStatus(const void *msg);    // mavlink_cesat_imu_status_t*
    // TODO: handleCesatAlgorithmStatus, handleCesatEstimatorState (M3)

signals:
    void fpgaStatusChanged();
    void imuStatusChanged();

private:
    int _fpgaVersion = 0;
    int _uartCount = 0;
    bool _fpgaOk = false;
    int _sensorPath = 0;
    int _carrierDetect = 0;

    bool _imu1Ok = false;
    bool _imu2Ok = false;
    bool _imu3Ok = false;
    bool _precImuOk = false;
    bool _stim300Ok = false;
    bool _baro1Ok = false;
    bool _baro2Ok = false;
};
