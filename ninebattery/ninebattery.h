#include "esphome.h"

class NinebatteryUART : public UARTDevice {
public:
    NinebatteryUART(UARTComponent *parent) : UARTDevice(parent) {}

    int16_t get_status() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_status, sizeof(cmd_get_status), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_status failed!");
            return -1000;
        }
        // https://github.com/etransport/ninebot-docs/wiki/ES2BMS#30-status-register

        //status resp (11):    5aa50222200430010086ff          0x5a 0xa5 0x02 0x22 0x20 0x04 0x30 0x01 0x00 0x86 0xff

        //  98 7654 3210
        //            01 1   - normal/discharging
        //0000 0100 0001 65  - charging
        //0010 0100 0001 577 - probably overvoltage
        //0010 0000 0001 513 - probably normal with warning


        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_status: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_remaining_capacity_perc() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_remaining_capacity_perc, sizeof(cmd_get_remaining_capacity_perc), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_remaining_capacity_perc failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_remaining_capacity_perc: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_remaining_capacity() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_remaining_capacity, sizeof(cmd_get_remaining_capacity), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_remaining_capacity failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_remaining_capacity: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_actual_capacity() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_actual_capacity, sizeof(cmd_get_actual_capacity), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_actual_capacity failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_actual_capacity: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_factory_capacity() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_factory_capacity, sizeof(cmd_get_factory_capacity), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_factory_capacity failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_factory_capacity: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_current() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_current, sizeof(cmd_get_current), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_current failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_current: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    int16_t get_voltage() {
        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_voltage, sizeof(cmd_get_voltage), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_voltage failed!");
            return -1000;
        }

        int16_t resp = response[8] << 8 | response[7];
        ESP_LOGD(TAG, "get_voltage: %u DF7=%02X DF8=%02X", resp, response[7], response[8]);
        return resp;
    }

    uint16_t cell_voltage[10] = {};
    uint16_t *get_cells_voltage() {
        uint8_t response[29] = {0};
        bool success = sendUartCommand(cmd_get_cells_voltage, sizeof(cmd_get_cells_voltage), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_cells_voltage failed!");
            return cell_voltage;
        }

        cell_voltage[0] = response[8] << 8 | response[7];
        cell_voltage[1] = response[10] << 8 | response[9];
        cell_voltage[2] = response[12] << 8 | response[11];
        cell_voltage[3] = response[14] << 8 | response[13];
        cell_voltage[4] = response[16] << 8 | response[15];
        cell_voltage[5] = response[18] << 8 | response[17];
        cell_voltage[6] = response[20] << 8 | response[19];
        cell_voltage[7] = response[22] << 8 | response[21];
        cell_voltage[8] = response[24] << 8 | response[23];
        cell_voltage[9] = response[26] << 8 | response[25];
        ESP_LOGD(TAG, "get_cells_voltage: %u DF7=%02X DF8=%02X", cell_voltage[0], response[7], response[8]);
        return cell_voltage;
    }

    uint8_t temp[2] = {};
    uint8_t *get_temperature() {

        uint8_t response[11] = {0};
        bool success = sendUartCommand(cmd_get_temperature, sizeof(cmd_get_temperature), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "get_temperature failed!");
            return temp;
        }

        memcpy(&temp, &response[7], 2 * sizeof(response[0]));

        ESP_LOGD(TAG, "get_temperature: DF7=%02X DF8=%02X", response[7], response[8]);

        return temp;
    }

    std::string get_serial() {
        uint8_t response[23] = {0};
        bool success = sendUartCommand(cmd_get_serial, sizeof(cmd_get_serial), response, sizeof(response));

        if(!success || !validate(response, sizeof(response))) {
            ESP_LOGW(TAG, "Reading battery serial failed!");
            return "";
        }
        char serial[15] = {};
        memcpy(&serial, &response[7], 14 * sizeof(response[0]));
        serial[14] = 0x00;
        ESP_LOGD(TAG, "Battery serial: %s", serial);

        return serial;
    }

    
private:
    const char *TAG = "NB";
    uint8_t cmd_get_status[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x30, 0x02, 0x89, 0xff,};
    uint8_t cmd_get_serial[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x10, 0x0e, 0x9d, 0xff,};
    uint8_t cmd_get_remaining_capacity_perc[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x32, 0x02, 0x87, 0xff,};
    uint8_t cmd_get_remaining_capacity[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x31, 0x02, 0x88, 0xff,};
    uint8_t cmd_get_actual_capacity[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x19, 0x02, 0xa0, 0xff,};
    uint8_t cmd_get_factory_capacity[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x18, 0x02, 0xa1, 0xff,};
    uint8_t cmd_get_current[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x33, 0x02, 0x86, 0xff,};
    uint8_t cmd_get_voltage[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x34, 0x02, 0x85, 0xff,};
    uint8_t cmd_get_cells_voltage[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x40, 0x14, 0x67, 0xff,};
    uint8_t cmd_get_temperature[10] = {0x5a, 0xa5, 0x01, 0x20, 0x22, 0x01, 0x35, 0x02, 0x84, 0xff,};

    // Checksum: 256-(HEAD+LEN+CMD+DATA)%256
    bool calcCRC(uint8_t *response, size_t len) {
        uint16_t crc = 0;
        for (int i = 2; i <= len - 3; i++) {
            crc = crc + response[i];
        }
        if (((response[len - 1] << 8) | response[len - 2]) == (0xFFFF ^ crc)) {
            ESP_LOGD(TAG, "CRC OK: 0x%04X / 0x%02X%02X", 0xFFFF ^ crc, response[len-1], response[len-2]);

            return true;
        }

        ESP_LOGW(TAG, "CRC NOT OK: 0x%04X / 0x%02X%02X (len %d)", 0xFFFF ^ crc, response[len-1], response[len-2], len);

        return false;
    }

    bool validate(uint8_t* response, size_t len) {
        if(response[0] != 0x5a || response[1] != 0xa5) {
            ESP_LOGW(TAG, "Got wrong UART response: %02X %02X %02X %02X...", response[0], response[1], response[2], response[3]);
            return false;
        }

        if(!calcCRC(response, len)) {

            return false;
        }

        return true;
    }

    bool sendUartCommand(uint8_t *command, size_t commandLen, uint8_t *response = nullptr, size_t responseLen = 0) {
        // Empty RX Buffer
        while (available()) {
            read();
        }

        // calculate CRC
//        command[commandLen - 1] = calcCRC(command, commandLen);

        write_array(command, commandLen);
        flush();

        if(response == nullptr) {
            return true;
        }

        return read_array(response, responseLen);
    }
};

NinebatteryUART *nbu;


class NinebatterySensor : public PollingComponent {
public:
    Sensor *s_status = new Sensor();
    Sensor *s_remaining_capacity_perc = new Sensor();
    Sensor *s_remaining_capacity = new Sensor();
    Sensor *s_actual_capacity = new Sensor();
    Sensor *s_factory_capacity = new Sensor();
    Sensor *s_current = new Sensor();
    Sensor *s_voltage = new Sensor();
    Sensor *s_power = new Sensor();
    Sensor *s_temperature_zone_0 = new Sensor();
    Sensor *s_temperature_zone_1 = new Sensor();

    Sensor *s_voltage_cell_0 = new Sensor();
    Sensor *s_voltage_cell_1 = new Sensor();
    Sensor *s_voltage_cell_2 = new Sensor();
    Sensor *s_voltage_cell_3 = new Sensor();
    Sensor *s_voltage_cell_4 = new Sensor();
    Sensor *s_voltage_cell_5 = new Sensor();
    Sensor *s_voltage_cell_6 = new Sensor();
    Sensor *s_voltage_cell_7 = new Sensor();
    Sensor *s_voltage_cell_8 = new Sensor();
    Sensor *s_voltage_cell_9 = new Sensor();

private:

public:
    NinebatterySensor(UARTComponent *parent, uint32_t update_interval) : PollingComponent(update_interval) {
        nbu = new NinebatteryUART(parent);
    }

    void setup() override {
    }

    void update() override {
        int16_t status = nbu->get_status();
        if (status != -1000) {
            s_status->publish_state(status);
        }
        int16_t remaining_capacity_perc = nbu->get_remaining_capacity_perc();
        if (remaining_capacity_perc != -1000) {
            s_remaining_capacity_perc->publish_state(remaining_capacity_perc);
        }
        int16_t remaining_capacity = nbu->get_remaining_capacity();
        if (remaining_capacity != -1000) {
            s_remaining_capacity->publish_state(remaining_capacity);
        }
        int16_t actual_capacity = nbu->get_actual_capacity();
        if (actual_capacity != -1000) {
            s_actual_capacity->publish_state(actual_capacity);
        }
        int16_t factory_capacity = nbu->get_factory_capacity();
        if (factory_capacity != -1000) {
            s_factory_capacity->publish_state(factory_capacity);
        }
        int16_t current = nbu->get_current();
        if (current != -1000) {
            s_current->publish_state(float(current)/100);
        }
        int16_t voltage = nbu->get_voltage();
        if (voltage != -1000) {
            s_voltage->publish_state(float(voltage)/100);
        }
        if(current != -1000 && voltage != -1000) {
            s_power->publish_state(float(voltage)/100 * float(current)/100);
        }
        uint8_t *temperature = nbu->get_temperature();
        if(temperature[0]) {
            s_temperature_zone_0->publish_state(temperature[0]-20);
            s_temperature_zone_1->publish_state(temperature[1]-20);
        }

        uint16_t *cells_voltage = nbu->get_cells_voltage();
        if(cells_voltage[0]) {
            s_voltage_cell_0->publish_state(cells_voltage[0]);
            s_voltage_cell_1->publish_state(cells_voltage[1]);
            s_voltage_cell_2->publish_state(cells_voltage[2]);
            s_voltage_cell_3->publish_state(cells_voltage[3]);
            s_voltage_cell_4->publish_state(cells_voltage[4]);
            s_voltage_cell_5->publish_state(cells_voltage[5]);
            s_voltage_cell_6->publish_state(cells_voltage[6]);
            s_voltage_cell_7->publish_state(cells_voltage[7]);
            s_voltage_cell_8->publish_state(cells_voltage[8]);
            s_voltage_cell_9->publish_state(cells_voltage[9]);
        }


    }
};

class NinebatterySerialSensor : public PollingComponent {
public:
    TextSensor *s_serial = new TextSensor();
private:
    std::string serialNumber;
public:
    NinebatterySerialSensor(uint32_t update_interval) : PollingComponent(update_interval) {
    }

    void setup() override {
    }

    void update() override {
        if (!serialNumber.empty()) {

            return;
        }
        serialNumber = nbu->get_serial();
        if (!serialNumber.empty()) {
            s_serial->publish_state(serialNumber);
        }
    }
};


class NinebatteryStatusSensor : public PollingComponent {
public:
    BinarySensor *s_00 = new BinarySensor();
    BinarySensor *s_01 = new BinarySensor();
    BinarySensor *s_02 = new BinarySensor();
    BinarySensor *s_03 = new BinarySensor();
    BinarySensor *s_04 = new BinarySensor();
    BinarySensor *s_05 = new BinarySensor();
    BinarySensor *s_06 = new BinarySensor();
    BinarySensor *s_07 = new BinarySensor();
    BinarySensor *s_08 = new BinarySensor();
    BinarySensor *s_09 = new BinarySensor();
    BinarySensor *s_10 = new BinarySensor();
    BinarySensor *s_11 = new BinarySensor();
    BinarySensor *s_12 = new BinarySensor();
    BinarySensor *s_13 = new BinarySensor();
    BinarySensor *s_14 = new BinarySensor();
    BinarySensor *s_15 = new BinarySensor();
private:
    std::int16_t status;
public:
    NinebatteryStatusSensor(uint32_t update_interval) : PollingComponent(update_interval) {
    }

    void setup() override {
    }

    void update() override {
        status = nbu->get_status();
        if (status == -1000) {
            return;
        }

        s_00->publish_state((status >> 0) & 1);
        s_01->publish_state((status >> 1) & 1);
        s_02->publish_state((status >> 2) & 1);
        s_03->publish_state((status >> 3) & 1);
        s_04->publish_state((status >> 4) & 1);
        s_05->publish_state((status >> 5) & 1);
        s_06->publish_state((status >> 6) & 1);
        s_07->publish_state((status >> 7) & 1);
        s_08->publish_state((status >> 8) & 1);
        s_09->publish_state((status >> 9) & 1);
        s_10->publish_state((status >> 10) & 1);
        s_11->publish_state((status >> 11) & 1);
        s_12->publish_state((status >> 12) & 1);
        s_13->publish_state((status >> 13) & 1);
        s_14->publish_state((status >> 14) & 1);
        s_15->publish_state((status >> 15) & 1);
    }
};