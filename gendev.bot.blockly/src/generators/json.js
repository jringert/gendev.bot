import * as Blockly from 'blockly';

export const jsonGenerator = new Blockly.Generator('JSON');

jsonGenerator.PRECEDENCE = 0;

jsonGenerator.scrub_ = function(block, code, thisOnly) {
  const nextBlock =
      block.nextConnection && block.nextConnection.targetBlock();
  if (nextBlock && !thisOnly) {
    return code + ',\n' + jsonGenerator.blockToCode(nextBlock);
  }
  return code;
};

jsonGenerator['myite'] = function(block) {
  const value = jsonGenerator.valueToCode(
    block, 'MEMBER_VALUE', jsonGenerator.PRECEDENCE);
  const thenBlock =
    jsonGenerator.statementToCode(block, 'MEMBERS_THEN');
  const elseBlock =
    jsonGenerator.statementToCode(block, 'MEMBERS_ELSE');

  return `if (${value}) {\n  ${thenBlock}\n} else {\n  ${elseBlock}\n}`;
};

jsonGenerator['move'] = function(block) {
  const value = block.getFieldValue('VALUE');
  return `drive("${value}");`;  
};

jsonGenerator['dist_compare'] = function(block) {
  const value = block.getFieldValue('DISTANCE');
  return [`distance < ${value}`, jsonGenerator.PRECEDENCE];
};

jsonGenerator['emgOff_pressed'] = function(block) {
  return ['emgOff == true', jsonGenerator.PRECEDENCE];
};
