package ru.galilov.totp;


import com.fazecast.jSerialComm.SerialPort;

public class Main {

    public static void main(String[] args) throws Exception {

        byte[] keyData = {
                23, // length of the following data block
                1, // save the key using index 0  (0..7 are available)
                6, // totp length in digits
                30, // totp step in seconds
                // key
                0x38, (byte) 0x93, (byte) 0xd0, 0x0f, 0x0a, (byte) 0xfd, (byte) 0xe0, 0x67,
                0x06, (byte) 0xf6, 0x4a, (byte) 0x9d, (byte) 0xbd, 0x58, (byte) 0x8f, (byte) 0x89,
                (byte) 0xd8, (byte) 0xab, (byte) 0xa8, (byte) 0xcc
        };

//        byte[] keyData1 = {
//                23, // length of the following data block
//                3, // save the key using index 3  (0..7 are available)
//                6, // totp length in digits
//                60, // totp step in seconds
//                // key
//                0x00, (byte) 0x93, (byte) 0xd0, 0x0f, 0x0a, (byte) 0xfd, (byte) 0xe0, 0x67,
//                0x06, (byte) 0xf6, 0x4a, (byte) 0x9d, (byte) 0xbd, 0x58, (byte) 0x8f, (byte) 0x89,
//                (byte) 0xd8, (byte) 0xab, (byte) 0xa8, (byte) 0xcc
//        };
        SerialPort comPort = SerialPort.getCommPort("com14");
        comPort.openPort();
        comPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING | SerialPort.TIMEOUT_READ_BLOCKING,
                500, 500);
        comPort.writeBytes(keyData, keyData.length);
        System.out.println(comPort.getInputStream().read());

        comPort.closePort();
    }
}
