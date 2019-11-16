# **Cerberus core specification**

## Table of Contents

## Cerberus Command Set

The supported commands are listed in this *Table*. Each command is described in this section.

| **Command**                      | **Command code** | Command description                                          |
| -------------------------------- | :--------------: | ------------------------------------------------------------ |
| Get                              |       0x00       | Gets version and the allowed commands supported by the current version of the Cerberus core. |
| Get Version                      |       0x01       | Gets the Cerberus core version.                              |
| Read Protection Status           |       0x02       | Read Protection status of the Cerberus core.                 |
| Get ID                           |       0x03       | Gets the Cerberus core ID.                                   |
| Read Device Public Key           |       0x11       | Get the device public key.                                   |
| Read Hash Code                   |       0x12       | Get the application hash code. The Get Hash Code command shall be valid only the Generate Key command to be executed. |
| Read Security Monitor Public Key |       0x13       | Get the Security Monitor Public Key. The Get Security Monitor Public Key command shall be valid only the Generate Key command to be executed. |
| Generate                         |       0x21       | Generate the Hash Code, Security Monitor Public Key,... which are used by the Security Monitor. |
| Sign                             |       0x31       | Create a signature for message. The Sign command shall be valid only he Generate command to be executed. |

**Note:**

1. If a denied command is received or an error occurs during the command execution, the core sends NACK byte and goes back to command checking.
2.  Get Hash Code and Get Security Monitor Public Key command is valid, only if the Generate command was executed.

Each packet is either accepted (ACK answer) or discarded (NACK answer):

* ACK = 0x79
* NACK = 0x1F

### Get command

The  Get command allows the user to get the version of the cerberus core and the supported commands. When the cerberus core receives the Get command, it transmits the core version and the supported command codes to the host, as show in *Figure*.

**Get command: host side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102152356102.png" alt="image-20191102152356102" style="zoom:67%;" />

**Get command: device side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102154308395.png" alt="image-20191102154308395" style="zoom:67%;" />



The Cerberus core sends the bytes as follows:

* Byte 1: 		    ACK 
* Byte 2:            N = 9 = the number of bytes to follow - 1 except current and ACKs.
* Byte 3:            Cerberus core version (0 < version < 255), example: 0x10 = version 1.0
* Byte 4:             0x00                   -  Get command.
* Byte 5:             0x01                   -  Get Version command.
* Byte 6:             0x02                    -  Get Protection Status command.
* Byte 7:             0x03                    -  Get ID command.
* Byte 8:             0x04                    -  Read Device Public Key command.
* Byte 9:             0x05                    -  Read Hash Code command.
* Byte 10:           0x06                    -  Read Security Monitor Public Key command.
* Byte 11:           0x11                    -  Generate command.
* Byte 12:           0x21                    -  Sign command.
* Last byte (15): ACK

### Get Version command

The Get version command is used to get the Cerberus core version. After receiving the command the core transmits the version to the host.

**Get version: host side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102160908472.png" alt="image-20191102160908472" style="zoom:67%;" />

**Get version: device side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102161435943.png" alt="image-20191102161435943" style="zoom:67%;" />



The Cerberus core sends the bytes as follows:

* Byte 1: 		    ACK 
* Byte 2:            Cerberus core version (0 < version < 255), example: 0x10 = version 1.0
* Last byte (3): ACK

### Read Protection Status command

This command is reserved for the future.

### Get ID command

The Get ID command is used to get the version of the chip ID (identification). When the Cerberus core receives the command, it transmits the product ID to the host

**Get ID: host side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102162210527.png" alt="image-20191102162210527" style="zoom: 50%;" />

**Get ID: device side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102162437557.png" alt="image-20191102162437557" style="zoom:67%;" />

The Cerberus core sends the bytes follows:

* Byte 1: 		    ACK 
* Byte 2,3:        PID
* Last byte (4): ACK

### Read Device Public Key

The Read Device Public Key command is used to read the device public key from the core handler.

When the core handler receives the Read Device Public Key command, it transmits ACK byte to the application. After the transmission of the ACK byte, it transmits the device public key to the host.

**Read Device Public Key: host side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102164156801.png" alt="image-20191102164156801" style="zoom:67%;" />

**Read Device Public Key: device side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102163847816.png" alt="image-20191102163847816" style="zoom:67%;" />

### Read Hash Code

The Read Hash Code command is used to read the application hash code (see Secure Boot documentation for more detail).

When the core handler receives the Read Hash Code command and the Hash Code is valid, it shall  transmit an ACK byte to the host, otherwise it transmits a NACK byte and aborts the command. When the hash code is valid, the core handler

* Sends a byte, N, which contains the number of bytes to be transmitted.
* Sends Hash Code.
* Sends an ACK byte.

**Read Hash Code: host side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102164640641.png" alt="image-20191102164640641" style="zoom:67%;" />

**Read Hash Code: device side**

<img src="/home/aion/.config/Typora/typora-user-images/image-20191102165106489.png" alt="image-20191102165106489" style="zoom:67%;" />

### Read Security Monitor Public Key

The Read Security Monitor Public Key command is used for reading the Security Monitor Public Key (see Secure Boot documentation for more detail).

When the keycore receives the Read Security Monitor Public Key command and the the Key is valid, it shall transmit an ACK byte to the host, otherwise it transmits a NACK byte and aborts command. When the security monitor public key is valid, the core handler

* Sends a byte, N, which contains the number of bytes to be transmitted.
* Sends Security Monitor Public Key.
* Sends an ACK byte.

### Generate

The Generate command is one of the most powerful commands. This command is used to generate the Hash Code, Public and Private Security Monitor Key,... (see Secure Boot documentation for more detail).

When the core handler receives the Generate command, it transmits an ACK byte to the host. After transmission ACK byte, the keycore waits for a size of the application payload N (four bytes, byte 1 is the size MSB and byte 4 is the LSB) and a checksum byte, it then checks the received data. 

If the received size is valid and the checksum is correct, the handler transmits an ACK byte, otherwise it transmits a NACK byte and aborts the command. When the size is valid and the checksum is correct, the keycore 

* receives segment 0:

  * gets bytes, N<sub>0</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 00; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).
  
  * If received data and checksum is valid, it transmits an ACK byte and next segment, otherwise it transmits a NACK byte and aborts the command.
  
* receives segment 1:

  * gets bytes, N<sub>1</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 01; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).

  * If received data and checksum is valid, it transmits an ACK byte and next segment, otherwise it transmits a NACK byte and aborts the command.

​	...

* receives segment n:

  * gets bytes, N<sub>n</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 03; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).

  * If received data and checksum is valid, it transmits an ACK byte , otherwise it transmits a NACK byte and aborts the command.

Note: N<sub>o</sub> + N<sub>1</sub> + ... + N<sub>n</sub> must equal N + 1.

After transmission application data bytes, Hash Code, Public and Private Security Monitor Key,... shall be generated by the keycore. If successfully, it transmits an ACK byte, otherwise it transmits a NACK byte and aborts the command.

**Generate: host side**



**Generate: device side**



### Sign

The Sign command is used to create a signature for message (see Secure Boot documentation for more detail). The Public Security Monitor and Private Security Monitor are used for generating a signature in this command. 

When the core handler receives the Sign command and the SM keys is valid, it transmits an ACK byte to the host, otherwise it transmits a NACK byte and aborts the command. When the Sign command is valid, the core handler waits for the number of bytes of message (four bytes, byte 1 is the size MSB and byte 4 is the LSB) and a checksum byte, it then checks the received data.

If the received data is valid and checksum is correct, the handler transmits an ACK byte, otherwise it transmits a NACK byte and aborts the command. When the size is valid and the checksum is correct, the keycore 

* receives segment 0:

  * gets bytes, N<sub>0</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 00; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).
  * If received data and checksum is valid, it transmits an ACK byte and next segment, otherwise it transmits a NACK byte and aborts the command.

* receives segment 1:

  * gets bytes, N<sub>1</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 01; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).
  * If received data and checksum is valid, it transmits an ACK byte and next segment, otherwise it transmits a NACK byte and aborts the command.

  ...

* receives segment n:

  * gets bytes, N<sub>n</sub>, which contains the number of bytes to be followed.
  * receives segment type byte = 03; Note: 00 - Segment start, 01 - Next segment, 03 - Last segment.
  * receives the application data and a checksum (XOR of N and of all data bytes).
  * If received data and checksum is valid, it transmits an ACK byte, otherwise it transmits a NACK byte and aborts the command.

Note: N<sub>o</sub> + N<sub>1</sub> + ... + N<sub>n</sub> must equal N + 1.

After transmission application data bytes, the keycore generates  a Signature,... If successfully, it transmits number of bytes of signature and signature, otherwise it transmits a NACK byte and aborts the command.

