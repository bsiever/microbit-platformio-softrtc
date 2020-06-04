# microbit-ble-testservice

Micro:bit Bluetooth Test Service project for the micro:bit (and compiled on Platform IO)

The service can be used to test BLE libraries on different platforms.

Be sure to use a recent version of the Micro:bit firmware (>=0253). See [https://microbit.org/get-started/user-guide/firmware/](https://microbit.org/get-started/user-guide/firmware/)


# Service 

| Props | Short desc | UUID | Long Description |
|-------|------------|------|------------------|
| R     |  Data Short    | 1d93b2f8-9239-11ea-bb37-0242ac130002 |  Contains ASCII digits 0-9: "0123456789"  (10 bytes) | 
| R     |  Data Packet   | 1d93b488-9239-11ea-bb37-0242ac130002 | Contains 20 bytes:"abcdefghijklmnopqrst" (full BLE packet) |
| R     |  Data Long     | 1d93b56e-9239-11ea-bb37-0242ac130002 | Contains 62 bytes: "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" (multiple packets) |
| RWn    | Short R/Wn Data  | 1d93b636-9239-11ea-bb37-0242ac130002 | For testing writes up to 20 bytes (readback to confirm)|
| RWr    | Short R/W Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | For testing writes up to 80 bytes (readback to confirm) |
| RWr | Client Disconnect | 1d93c1e4-9239-11ea-bb37-0242ac130002 | Time (in ms) until client will disconnect intentionally |
| RWr | Client Reset (hard disconnect) | 1d93c2c0-9239-11ea-bb37-0242ac130002| Time (in ms) until client will disconnect intentionally |
| RW | Auth Permissions | 1d93b7c6-9239-11ea-bb37-0242ac130002 | 4 ASCII bytes; including an "R" allows Read and "W" allows write |
| RaWa | Auth Data | 1d93b884-9239-11ea-bb37-0242ac130002 | Data for authorization test (8 bytes) |
| RWn    | Short R Data  |  1d93c432-9239-11ea-bb37-0242ac130002  | Only 1 byte of data ("-"); For testing Descriptors  ;  Includes writeable auxiliary (0x2900==0002 (little endian, so 02 is first byte)), User description (the writeable auxiliary; "Test Test Test Test " initially 20 bytes), Server  Config (also writeable, initially 0), 2 bytes, Presentation Format (GattCharacteristic::BLE_GATT_FORMAT_SINT16, 3, GattCharacteristic::BLE_GATT_UNIT_AREA_SQUARE_METRES, 1, 0) = [0x0e, 0x03, 0x10, 0x27, 0x01, 0x00, 0x00], and A special attribute (0x2929 = 0 (2 bytes), little endian)|
| RWr | Notifiable counter1 period | 1d93b6fe-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Notifiable counter1 | 1d93bb2c-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Notifiable counter2 period | 1d93bbea-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Notifiable counter2 | 1d93bc9e-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter1 period | 1d93bd52-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter1 | 1d93be06-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter2 period | 1d93bec4-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter2 | 1d93bf82-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |

## Property Abbreviations

| Abbreviation | Meaning | 
|:------------:|:--------|
| R            | Read |
| Wn           | Write without response |
| Wr           | Write with response (ack if less than limit; Nack if over limit) |
| Wa           | Authorized Write (may be rejected) |
| N            | Notifiable |
| I            | Indicatable |

Writing a new timer period resets the count and starts the timer with the new period.  The count will increase to 1 at the end of the first period. Timer update frequency is about 50ms (i.e., setting time to less than 50ms will be essentially update every 50ms)

Numeric values should be little-endian

# For Future Modifications

* Update `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` for
  * Advertising
  * GAP Name
* Update `src/BLETestService.cpp` (and possibly `src/BLETestService.h`) for newer services
* Update `lib/ble-nrf51822/source/btle/btle.cpp` for long writes
  * `MAX_ATTR_SIZE = 80;` defines size of max attribute
  

# Misc. Notes

* Platform IO Project 
* Resulting Hex file is in `.pio/build/bbcmicrobit/firmware.hex`
* Modified `lib/ble/UUID.h` for proper byte ordering in UUIDs for services (LSB first).  New constructor to allow either LSB or MSB depending on need. (Some BTLE stack things need one order while mbed things need another)
* Modified `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` to:
  * Change device name in GAP
  * Change advertising as needed
  * `.hex` file will be built  in `.pio/build/bbcmicrobit`
  * Currently requires older version of PlatformIO's nordicnrf51 library (3.0.1; newer versions seem to have compatibility problems, probably due to use of newer and incompatible mbed versions)
* Added `src/BLETestService.cpp` and `src/BLETestService.h` for the test service.
* `platformio.ini` specifies baud rate / etc.
