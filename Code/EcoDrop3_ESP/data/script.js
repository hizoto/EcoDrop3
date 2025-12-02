const ws = new WebSocket(`ws://${window.location.hostname}/ws`);

// Log-Ausgabe ins HTML-Element
const logDiv = document.getElementById('log');

// Wenn eine Nachricht vom ESP kommt
ws.onmessage = (event) => {
  console.log("Server:", event.data);
  if (logDiv) {
    logDiv.innerHTML += event.data + "<br>";
    logDiv.scrollTop = logDiv.scrollHeight; // Auto-Scroll
  }
};

// Wenn Verbindung geöffnet wird
ws.onopen = () => console.log("WebSocket verbunden");

// Wenn Verbindung geschlossen wird
ws.onclose = () => console.log("WebSocket getrennt");

// Funktion, um Befehle an den ESP zu schicken
function sendCommand(cmd) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(cmd);
  } else {
    console.warn("WebSocket nicht verbunden!");
  }
}

// Elemente im UI
const tempSpan = document.getElementById("temp");
const voltageSpan = document.getElementById("voltage");
const currentSpan = document.getElementById("current");
//const chargingCurrentSpan = document.getElementById("chargingCurrent");
//const chargingVoltageSpan = document.getElementById("chargingVoltage");

// Funktion, um Sensordaten vom ESP zu holen
async function fetchSensorData() {
  try {
    const response = await fetch(`http://${window.location.hostname}/sensors`);
    if (!response.ok) throw new Error("HTTP " + response.status);
    const data = await response.json();

    if (tempSpan) tempSpan.textContent = data.temp + " °C";
    if (voltageSpan) voltageSpan.textContent = data.voltage + " V";
    if (currentSpan) currentSpan.textContent = data.current + " A";
  } catch (err) {
    console.warn("Konnte Sensordaten nicht laden:", err);
  }
}

// alle 5s aktualisieren
setInterval(fetchSensorData, 5000);
fetchSensorData();