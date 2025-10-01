#include <Arduino.h>
#include <FS.h>         // NUR für File/Listing-APIs
#include <LittleFS.h>

static void listFS() {
  Serial.println("LittleFS listing:");
  File root = LittleFS.open("/");
  if (!root) { Serial.println("  open('/') failed"); return; }
  File f = root.openNextFile();
  if (!f) { Serial.println("  (empty)"); return; }
  while (f) {
    Serial.printf("  %s (%u bytes)\n", f.name(), (unsigned)f.size());
    f = root.openNextFile();
  }
}

void testLittleFS() {
  bool ok = LittleFS.begin(true); // formatOnFail = true
  Serial.printf("LittleFS.begin(): %s\n", ok ? "OK" : "FAIL");
  if (!ok) return;

  // Kapazität/Belegung
  size_t total = LittleFS.totalBytes();
  size_t used  = LittleFS.usedBytes();
  Serial.printf("LittleFS total: %u bytes, used: %u bytes\n", (unsigned)total, (unsigned)used);

  // Falls index fehlt -> anlegen (Proof)
  if (!LittleFS.exists("/index.ssssssssssssssssssssssssssssssssssssssssssssssssshtml")) {
    Serial.println("'/index.html' fehlt -> erstelle Testdatei");
    File w = LittleFS.open("/index.html", FILE_WRITE);
    if (w) { w.print("<h1>It works (LittleFS)</h1>"); w.close(); }
    else   { Serial.println("Konnte /index.html nicht schreiben!"); }
  }

}