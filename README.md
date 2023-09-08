# RT_Light-Decoder
A DCC accessory decoder for controlling light effects.


CV control of various light accessories.


For each output pin need 9 CVs for data storage

           output pin CV
           1   2   3   4   ...

data

mode       33  43  53  63  73  ... 
ontime     34  44  54  64
ontimeX    35  45  55  65
offtime    36  46  56  66
offtimeX   37  47  57  67
outputPin  38  48  58  68
outputPin2 39  49  59  69
fadein     40  50  60  70
fadeout    41  51  61  71

mode
  0 continuous (stays on until off command received)
  1 oneshot (on for ontime once) ms / ontimeX
  2 flashing (alternate ontime offtime until off command received)
  3 flash/fade (alternate ontime offtime until off command received fade in/out
  4 flash alternate (requires two output pins)
  5 strobe double

ontime offtime
  time in ms / on__X for on or off functions

ontimeX offtimeX
  multiplier for on or off times


