import * as Blockly from 'blockly';


export const blocks = Blockly.common.createBlockDefinitionsFromJsonArray([
{
  'type': 'myite',
  'message0': 'if %1 then %2 else %3',
  'args0': [
    {
      'type': 'input_value',
      'name': 'MEMBER_VALUE',
    },
    {
      'type': 'input_statement',
      'name': 'MEMBERS_THEN',
    },
    {
      'type': 'input_statement',
      'name': 'MEMBERS_ELSE',
    },
  ],
  'previousStatement': null,
  'colour': 230,
},
{
  "type": "move",
  "message0": "Move %1",
  "args0": [
    {
      "type": "field_dropdown",
      "name": "VALUE",
      "options": [
        ["FORWARD", "forward"],
        ["LEFT", "left"],
        ["RIGHT", "right"],
        ["BACKWARD", "backward"],
        ["STOP", "stop"],
      ]
    }
  ],
  "previousStatement": null,
  "colour": 355
},
{
  'type': 'dist_compare',
  'message0': 'distance < %1',
  'args0': [
    {
      'type': 'field_input',
      'name': 'DISTANCE',
      'text': '',
    },
  ],
  'output': null,
  'colour': 110,
},
{
  'type': 'emgOff_pressed',
  'message0': 'emgOff',
  'output': null,
  'colour': 110,
},
{
  'type': 'doWork_pressed',
  'message0': 'doWork',
  'output': null,
  'colour': 110,
}
]);
