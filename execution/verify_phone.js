const assert = require('assert');
global.Pebble = { addEventListener: function () {} };
const logic = require('../watchface/src/pkjs/index.js');
const settings = require('../watchface/src/pkjs/settings.js');
assert.deepStrictEqual(settings.settingsMessage('{"SETTING_PALETTE":"2","SETTING_FONT":"1","SETTING_LOCATION":"7","SETTING_DATE":"3"}'),
  { SETTING_PALETTE: 2, SETTING_FONT: 1, SETTING_LOCATION: 7, SETTING_DATE: 3 });
assert.deepStrictEqual(settings.settingsMessage('{"SETTING_PALETTE":9,"SETTING_FONT":-1,"SETTING_LOCATION":8,"SETTING_DATE":9}'),
  { SETTING_PALETTE: 0, SETTING_FONT: 0, SETTING_LOCATION: 0, SETTING_DATE: 0 });
assert.deepStrictEqual(settings.settingsMessage('bad json'),
  { SETTING_PALETTE: 0, SETTING_FONT: 0, SETTING_LOCATION: 0, SETTING_DATE: 0 });
assert.strictEqual(logic.sceneForCode(0), 0);
assert.strictEqual(logic.sceneForCode(63), 1);
assert.strictEqual(logic.sceneForCode(73), 2);
assert.strictEqual(logic.sceneForCode(3), 3);
assert.strictEqual(logic.sceneForCode(undefined), -1);
console.log('PASS clear, rain, snow, cloud, and missing weather');
