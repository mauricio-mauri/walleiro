#include "DebugLog.h"

// ─── Instância global ────────────────────────────────────
DebugLog Debug;

// ─── Inicialização ───────────────────────────────────────
void DebugLog::begin(const char* ssid, const char* pass) {
  Serial.begin(115200);

  // Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);

  // Servidor web
  _server.on("/", [this]() {
    _server.send(200, "text/html", renderPage());
  });
  _server.begin();

  delay(500);
  Serial.println();
  Serial.print("DebugLog: AP \"");
  Serial.print(ssid);
  Serial.print("\", IP ");
  Serial.println(WiFi.softAPIP());
}

// ─── write (interface Print) ─────────────────────────────
size_t DebugLog::write(uint8_t c) {
  Serial.write(c);     // USB
  push((char)c);       // buffer circular
  return 1;
}

// ─── Push: acumula caracteres, fecha linha no '\n' ──────
void DebugLog::push(char c) {
  static char linha[LINE_LEN];
  static size_t i = 0;

  if (c == '\n') {
    linha[i] = '\0';
    strncpy(_buffer[_head], linha, LINE_LEN);
    _head  = (_head + 1) % MAX_LINES;
    if (_count < MAX_LINES) _count++;
    i = 0;
  } else if (i < LINE_LEN - 1) {
    linha[i++] = c;
  }
  // se i == LINE_LEN-1, descarta o caractere (overflow seguro)
}

// ─── Atende requisições HTTP ────────────────────────────
void DebugLog::handleClient() {
  _server.handleClient();
}

// ─── Página HTML do log ─────────────────────────────────
String DebugLog::renderPage() {
  String html = F(
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta charset='UTF-8'>"
    "<meta http-equiv='refresh' content='1'>"
    "<style>"
    "body{background:#1a1a1a;color:#0f0;font:14px/1.4 monospace;padding:16px}"
    "</style>"
    "</head>"
    "<body>"
  );

  // Percorre o buffer na ordem cronológica
  size_t start = (_count < MAX_LINES) ? 0 : _head;
  size_t n     = (_count < MAX_LINES) ? _count : MAX_LINES;

  for (size_t i = 0; i < n; i++) {
    size_t idx = (start + i) % MAX_LINES;
    html += _buffer[idx];
    html += '\n';
  }

  html += F("</body></html>");
  return html;
}
