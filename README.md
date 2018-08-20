# sample_dvb_api

This is a simple tuning tool for ISDB-S/ISDB-T using Linux DVB API tuners.

# How to build

    git clone https://github.com/katsuster/sample_dvb_api
    cd sample_dvb_api
    autoreconf -fi
    ./configure
    make

# How to Use

    ISDB-S
    
    sample_dvb adapter S [BS|CS110] ch TS-ID
    
    ISDB-T
    
    sample_dvb adapter T ch

'adapter' specify the number X of DVB API device /dev/dvb/adapterX/.
Please specify 0 if you use /dev/dvb/adapter0.

'ch' specify the physical channel number.

For ISDB-S:

* Specify transponder number X of BS-X or NDX
  * Please refer the satellite transponders list that was publised
  from Ministry of Internal Affairs and Communications
* Available range is BS-1 to BS-23 (for BS), ND1 to ND24 (for CS110)
  * Please specify only number part, not need 'BS' or 'ND' prefix
* Center IF frequency of BS ch1 (BS-1) is 1049480kHz (1.049GHz)
  * Local-IF is typically 10.678GHz for BS and CS110

For ISDB-T:

* Available range is 13 to 52.
* Center frequency of ch13 is 473142857 Hz (473MHz) of ISDB-T
  * Channel bandwidth is 6MHz, so ch14 is 479MHz, ch15 is 485MHz,
  ..., ch52 is 707MHz

'TS-ID' specify the unique ID of satellite network of BS or CS110.
Please search the information or refer the standard documents
such as ARIB TR-15.
