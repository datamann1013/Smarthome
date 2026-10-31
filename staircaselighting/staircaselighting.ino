// Stair lighting v1 — "make it work"
// 12 steps, 2 PIRs, 16-ch relay board (channels 1-12), hard on/off.
// Non-blocking state machine. Direction locked per cycle (no mid-cycle flip).

// ---------- Config ----------
const uint8_t  STEP_COUNT       = 12;
const unsigned STEP_DELAY_MS    = 250;     // gap between steps (12 steps ~= 3s sweep)
const unsigned HOLD_MS          = 20000;   // on-time after last motion
const unsigned MAX_ON_MS        = 300000;  // safety cutoff, 0 = off
const unsigned WARMUP_MS        = 45000;   // ignore PIRs right after power-on
const bool     RELAY_ACTIVE_LOW = true;    // most 12V relay boards trigger on LOW
const bool     USE_PIRS         = true;    // false = bench test via serial only
const bool     BOOT_TEST        = true;    // 1->12 chase at power-up

// ---------- Pin map (Uno) ----------
// Step 1 = bottom. Relays IN1..IN12 on D2..D12 + A0. D13 unused (bootloader blink).
static const uint8_t RELAY_PIN[STEP_COUNT] = {2,3,4,5,6,7,8,9,10,11,12,A0};
static const uint8_t PIR_BOTTOM = A1;
static const uint8_t PIR_TOP    = A2;

// ---------- State machine ----------
enum State { IDLE, SWEEP_ON, HOLD, SWEEP_OFF };
static State    state        = IDLE;
static bool     dirUp        = true;   // locked for the whole cycle
static int8_t   stepIdx      = 0;      // next step to act on
static unsigned lastStepMs   = 0;
static unsigned holdStartMs  = 0;
static unsigned cycleStartMs = 0;      // for MAX_ON_MS
static bool     bottomActive = false;
static bool     topActive    = false;

// ---------- Relay helper ----------
inline void setStep(uint8_t i, bool on) {
  digitalWrite(RELAY_PIN[i], (on == RELAY_ACTIVE_LOW) ? LOW : HIGH);
}
void allSteps(bool on) {
  for (uint8_t i = 0; i < STEP_COUNT; i++) setStep(i, on);
}

// One-shot chase 1->12. Blocking — only used at boot and for the 't' test.
void chase() {
  allSteps(false);
  for (uint8_t i = 0; i < STEP_COUNT; i++) {
    setStep(i, true);
    delay(STEP_DELAY_MS);
  }
  delay(300);
  allSteps(false);
}

void printStatus() {
  Serial.print(F("state="));
  Serial.print(state == IDLE ? "IDLE" : state == SWEEP_ON ? "SWEEP_ON" : state == HOLD ? "HOLD" : "SWEEP_OFF");
  Serial.print(F(" dir="));  Serial.print(dirUp ? F("UP") : F("DOWN"));
  Serial.print(F(" idx="));  Serial.print(stepIdx);
  Serial.print(F(" bot="));  Serial.print(bottomActive);
  Serial.print(F(" top="));  Serial.println(topActive);
}

// Read PIRs + serial. Returns true if motion seen this tick.
bool readMotion() {
  bottomActive = false;
  topActive    = false;
  if (USE_PIRS && millis() >= WARMUP_MS) {
    bottomActive = digitalRead(PIR_BOTTOM) == HIGH;
    topActive    = digitalRead(PIR_TOP)    == HIGH;
  }
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'u': bottomActive = true; break;            // fake bottom motion
      case 'd': topActive    = true; break;            // fake top motion
      case 'o': allSteps(true);  state = HOLD; holdStartMs = cycleStartMs = millis(); Serial.println(F("all on -> HOLD")); break;
      case 'x': allSteps(false); state = IDLE; Serial.println(F("all off -> IDLE")); break;
      case 't': chase(); break;
      case '?': printStatus(); break;
      default:  break;   // ignore \r \n space etc.
    }
  }
  return bottomActive || topActive;
}

// Start a fresh cycle in the given direction.
void startCycle(bool up) {
  dirUp = up;
  stepIdx = up ? 0 : STEP_COUNT - 1;
  state = SWEEP_ON;
  lastStepMs = cycleStartMs = millis();
  setStep(stepIdx, true);                 // light the first step right away
  stepIdx += up ? 1 : -1;
  Serial.print(F("-> SWEEP_ON ")); Serial.println(dirUp ? F("UP") : F("DOWN"));
}

// Advance the active sweep by one step if STEP_DELAY_MS has elapsed.
void tickSweep() {
  if (millis() - lastStepMs < STEP_DELAY_MS) return;
  lastStepMs = millis();

  if (state == SWEEP_ON) {
    if ((dirUp && stepIdx >= STEP_COUNT) || (!dirUp && stepIdx < 0)) {
      state = HOLD;
      holdStartMs = millis();
      Serial.println(F("-> HOLD"));
      return;
    }
    setStep(stepIdx, true);
    stepIdx += dirUp ? 1 : -1;
  }
  else if (state == SWEEP_OFF) {
    if ((dirUp && stepIdx >= STEP_COUNT) || (!dirUp && stepIdx < 0)) {
      state = IDLE;
      Serial.println(F("-> IDLE"));
      return;
    }
    setStep(stepIdx, false);
    stepIdx += dirUp ? 1 : -1;
  }
}

void setup() {
  for (uint8_t i = 0; i < STEP_COUNT; i++) pinMode(RELAY_PIN[i], OUTPUT);
  allSteps(false);                        // lights off at boot (NO contacts)
  if (USE_PIRS) {
    pinMode(PIR_BOTTOM, INPUT);
    pinMode(PIR_TOP,    INPUT);
  }
  Serial.begin(115200);
  if (BOOT_TEST) chase();
  Serial.println(F("stair v1 ready"));
}

void loop() {
  bool motion = readMotion();

  // Safety: stuck sensor / runaway cycle.
  if (MAX_ON_MS && state != IDLE && millis() - cycleStartMs > MAX_ON_MS) {
    allSteps(false);
    state = IDLE;
    Serial.println(F("SAFETY: max-on reached, all off"));
    return;
  }

  if (motion) {
    if (state == IDLE) {
      startCycle(bottomActive);           // tie-break: bottom wins
    }
    else if (state == SWEEP_OFF) {
      // Reignite: re-sweep on from the start, same direction. Already-on steps stay on.
      state = SWEEP_ON;
      stepIdx = dirUp ? 0 : STEP_COUNT - 1;
      lastStepMs = millis();
      setStep(stepIdx, true);
      stepIdx += dirUp ? 1 : -1;
      Serial.println(F("-> SWEEP_ON (reignite)"));
    }
    else if (state == HOLD) {
      holdStartMs = millis();             // extend hold
    }
    // SWEEP_ON: keep sweeping, nothing to do.
  }

  // Hold -> start switching off in the direction we walked.
  if (state == HOLD && millis() - holdStartMs >= HOLD_MS) {
    state = SWEEP_OFF;
    stepIdx = dirUp ? 0 : STEP_COUNT - 1;
    lastStepMs = millis();
    Serial.print(F("-> SWEEP_OFF ")); Serial.println(dirUp ? F("UP") : F("DOWN"));
  }

  tickSweep();
}
