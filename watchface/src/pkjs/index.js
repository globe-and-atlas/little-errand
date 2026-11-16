/* Phone weather changes the town scene; the courier and clock run offline. */
var REFRESH_MS = 30 * 60 * 1000;

function sceneForCode(code) {
  if (typeof code !== 'number' || !isFinite(code)) return -1;
  if (code === 0) return 0;
  if (code >= 71 && code <= 77 || code >= 85 && code <= 86) return 2;
  if (code >= 51 && code <= 67 || code >= 80 && code <= 82 ||
      code >= 95 && code <= 99) return 1;
  return 3;
}

function unavailable() {
  Pebble.sendAppMessage({ WEATHER: -1 });
}

function fetchWeather(latitude, longitude) {
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' +
    latitude.toFixed(4) + '&longitude=' + longitude.toFixed(4) +
    '&current=weather_code';
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.timeout = 12000;
  req.onload = function () {
    if (req.status !== 200) return unavailable();
    try {
      var payload = JSON.parse(req.responseText);
      var state = sceneForCode(payload && payload.current &&
                               payload.current.weather_code);
      Pebble.sendAppMessage({ WEATHER: state });
    } catch (error) {
      console.log('weather parse failed: ' + error);
      unavailable();
    }
  };
  req.onerror = unavailable;
  req.ontimeout = unavailable;
  req.send();
}

function refresh() {
  if (!navigator.geolocation) return unavailable();
  navigator.geolocation.getCurrentPosition(function (position) {
    fetchWeather(position.coords.latitude, position.coords.longitude);
  }, function (error) {
    console.log('location failed: ' + error.message);
    unavailable();
  }, { timeout: 12000, maximumAge: REFRESH_MS });
}

Pebble.addEventListener('ready', function () {
  require('./settings').initialize(function () {
    refresh();
    setInterval(refresh, REFRESH_MS);
  });
});

if (typeof module !== 'undefined')
  module.exports = { sceneForCode: sceneForCode };
