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
