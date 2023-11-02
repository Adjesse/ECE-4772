import struct

#0x100 EngineRPM
#0x101 VehicleSpeed
#0x102 ThrottlePosition
#0x103 Engine_CoolantTemp
#0x104 Fuel_Level
#0x105 Check_Engine 
#0x106 Vehicle_Status
#0x107 O2_Sensor


#Define the CAN data
can_messages = [
    (0x100, bytes.fromhex('0CAF')), 
    (0x101, bytes.fromhex('3A')),
    (0x102, bytes.fromhex('64')),
    (0x103, bytes.fromhex('A8')),
    (0x104, bytes.fromhex('1F')),
    (0x105, bytes.fromhex('01')),
    (0x106, bytes.fromhex('03')),
    (0x107, bytes.fromhex('030A')),
]

#Create and write the binary input file
with open('sample_can_data.bin', 'wb') as file:
    for msg_id, data in can_messages:
        msg_len = len(data)
        file.write(struct.pack('>IB', msg_id, msg_len))
        file.write(data)

print("Binary input file 'sample_can_data.bin' created.")
