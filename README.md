# audio_bt_mesh
The project set-up a BT mesh network to manage a secondary audio mesh
on different technologies such as BLE Audio, DECT, Nearlink or other.
## core function
The core function of the control layer is to manage the push2talk function
and to set-up the audio mesh system.
## BT Mesh PTT model
### Roles
#### Provisioner
The provisioner takes care to set-up the address inside the network.
#### Client
The client checks whether a talk request is granted or not.
#### Server
The server includes the talk floor management such as who is talking and 
assign a talking slot
## Build firmware
The firmware is buildable once for all three roles. Either 
- Provisioner + Client (Lead Node)
- Client
- Server
- Provisioner + Server (Lead Node)
- Provisioner + Server + Client (Lead Node)
