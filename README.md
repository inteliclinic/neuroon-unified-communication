# Neuroon Unified Communication (NUC) #

NUC is a plain C software module used for communication with Neuroon mask. It contains a set of functions and structures for generating and interpreting bluetooth frames.

## Getting Started ##

The best way to add NUC module to the project is to clone the repository via git as a submodule. It is also possible to put downloaded NUC module into project directory, but it requires manual update of module. NUC files should be linked into the project using e.g. CMake , Makefile.

## Deployment and examples ##

To use NUC functions and data structures in a project include one or more headers from API directory:
- ic\_dfu.h - functions provided with this file populates memory with data understandable by Neuroon mask in DFU mode and make mask enter DFU mode
- ic\_frame\_handle.h - access to data structures used to build bluetooth frames
- ic\_low\_level\_control.h - functions for building bluetooth frames which control Neuroon mask
- ic\_version.h - NUC version getters

### Examples: ###
- Function push\_data\_CMD0Frame takes raw bluetooth frame data and length of this frame in bytes, emits signal (to global signal system) with frame functional payload (devices parameters, configuration, etc.) and returns true or false depending on success or fail in frame validation.

```c
  bool push_data_CMD0Frame(uint8_t *data, uint16_t len){
    u_cmdFrameContainer cmd_frame_container;
    memcpy((void *)&cmd_frame_container, (void *)data, len); //copy raw frame data to local
    cmd_frame_container structure

    if (!neuroon_cmd_frame_validate(data, len)) //CRC8 check
      return false;

    uint32_t fp = 0;
    e_ccSignalType signal = CC_NO_DATA;

    switch(cmd_frame_container.frame.cmd){ //check what kind of frame is it
      case RGB_LED_CMD:
        signal = CC_RGB_CMD;
        rgb_cmd = cmd_frame_container.frame.payload.rgb_cmd;
        fp = (uint32_t)get_rgb_cmd;
        break;
      (...)
      default:
        break;
    }
    cc_emit(signal, fp); //emit signal with functional payload (function from outside NUC module)
    return true;
  }
```

- Function rgb\_both\_sin\_white\_max\_2s is an example of simple RGB LEDs configuration via bluetooth. Neuroon should execute on both LEDs sinusoidal function (2 seconds period) with max intensity and white light.

```c
  static void rgb_both_sin_white_max_2s(uint16_t frame_id){
    char array[CLI_BLE_FRAME_SIZE];
    memset(array, 0, sizeof(array));
    size_t len = CLI_BLE_FRAME_SIZE;
    rgb_led_set_func(array, &len, RGB_LED_SIDE_BOTH, FUN_TYPE_SIN_WAVE, RGB_LED_COLOR_WHITE, DEV_MAX_INTENSITY, DEV_INF_DURATION, 20, frame_id); //uppercase expressions are NUC's macros; 8th parameter is a given function period (in houndreds of milliseconds)
    ble_send_frame((uint8_t*)array, (uint16_t)len); //send frame via bluetooth (function from outside NUC module)
  }
```

## Contributing ##

Soon...

## Versioning ##

There is bash script update-version.sh in NUC's main directory which can set NUC version (by automatic generation of version.h file based on given information) in accordance with a pattern given in ic\_version.h file.

This method of setting NUC version is only temporary and will be changed.

NUC version getters are in ic\_version.h file.

## Authors ##

- Paweł Kaźmierzewski _p.kazmierzewski@inteliclinic.com_
- Wojtek Węclewski    _w.weclewski@inteliclinic.com_

## License ##

This project is licensed under the MIT License - see the LICENSE file for details.
