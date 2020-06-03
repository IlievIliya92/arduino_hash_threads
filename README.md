### Build the project
    cd [PROJECT_PATH]]/pi_arduino/arduino
    source aconfigure
    acheck_requirements
    abuild

### Upload to the target
    aupload [name_of_the_serial_device]

* The name_of_the_serial_device argument is optional.
  The supload script will look for serial devices connected to the
  host pc and upload to the first found. In case you have more than
  one connected it is mandatory to specify the name of the serial device.

