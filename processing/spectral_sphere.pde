import oscP5.*;
import netP5.*;
import java.awt.*;


OscP5 oscP5;

float leftOpen = 0;
float rightOpen = 0;
float leftThumb = 0;
float rightThumb = 0;

int currentNote = 60;  // default MIDI
float visualEnergy = 0.0;
int activeNotes = 0;

float noteVelocity = 0;

float attack = 0.05;
float release = 0.8;

float colorEnergy = 0;
float smoothDist = 0;
float smoothVelocity = 0;

float[] latitudes;
float[] longitudes;

int cols = 40;
int rows = 40;

void setup() {
  size(400, 400, P3D);
  surface.setResizable(false);
  surface.setTitle("Spectral Sphere");
  
  
  Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
  
  int offsetX = -420; 
  int offsetY = 0;
  
  surface.setLocation(
      screen.width - width - offsetX,
      screen.height - height - offsetY
    );

  int maxParticles = 1500;
  
  latitudes = new float[maxParticles];
  longitudes = new float[maxParticles];
  
  for (int i = 0; i < maxParticles; i++) {
    latitudes[i] = acos(1 - 2 * random(1));
    longitudes[i] = TWO_PI * random(1);
  }
  
  frameRate(60);
  oscP5 = new OscP5(this, 9001);
}

void oscEvent(OscMessage msg) {

  println(msg);  // debug globale

  if (msg.checkAddrPattern("/hand/left/open")) {
    float val = msg.get(0).floatValue();
    leftOpen = val;
    println("LEFT OPEN:", val);
  }

  else if (msg.checkAddrPattern("/hand/right/open")) {
    float val = msg.get(0).floatValue();
    rightOpen = val;
    println("RIGHT OPEN:", val);
  }

  else if (msg.checkAddrPattern("/hand/left/thumb")) {
    float val = msg.get(0).floatValue();
    leftThumb = val;
    println("LEFT THUMB:", val);
  }

  else if (msg.checkAddrPattern("/hand/right/thumb")) {
    float val = msg.get(0).floatValue();
    rightThumb = val;
    println("RIGHT THUMB:", val);
  }

  else if (msg.checkAddrPattern("/noteON")) {
    currentNote = msg.get(0).intValue();
    noteVelocity = msg.get(1).intValue() / 127.0;
    activeNotes++;
    println("LEN:", msg.arguments().length);
  }
  
  else if (msg.checkAddrPattern("/noteOFF")) {
    activeNotes = max(0, activeNotes - 1);
    println("NOTE OFF");
  }

  else if (msg.checkAddrPattern("/attack")) {
    attack = msg.get(0).floatValue();
  }
  
  else if (msg.checkAddrPattern("/releasee")) {
    release = msg.get(0).floatValue();
  }

}

void draw() {
  background(0);
  
  boolean noteActive = activeNotes > 0;

  translate(width/2, height/2);
  rotateY(frameCount * 0.01);
  rotateX(frameCount * 0.006);

  float lpf = leftOpen;
  float flanger = leftThumb;
  float chorus = rightOpen;
  float distBase = rightThumb;
  
  float distortion = distBase;
  distortion = constrain(distortion, 0, 1);
  
  // curva non lineare (fondamentale)
  distortion = pow(distortion, 1.8) * visualEnergy;
  
  smoothDist = lerp(smoothDist, distortion, 0.05);
  smoothVelocity = lerp(smoothVelocity, noteVelocity, 0.05);

  
  if (!noteActive) {
    distortion = 0;
    flanger = 0;
    chorus = 0;
  }

  if (noteActive) {
    float atkSpeed = 0.05 + (1.0 - attack) * 0.4;
    visualEnergy = lerp(visualEnergy, 1.0, atkSpeed);
  } else {
    float relSpeed = 1.0 - (0.85 + release * 0.1);
    visualEnergy *= (1.0 - relSpeed);
  }
  
  float colorSpeed = noteActive ? 0.1 : 0.03;
  colorEnergy = lerp(colorEnergy, visualEnergy, colorSpeed);

    
  if (!noteActive && visualEnergy < 0.02) {
    visualEnergy = 0;
  }

  float sphereRadius = min(width, height) * 0.30;
  float velocityFloor = 0.2;
  float velocityResponse = pow(smoothVelocity, 0.8); 
  float velocityScale = velocityFloor + (1.0 - velocityFloor) * velocityResponse;
  float baseRadius = sphereRadius * visualEnergy * velocityScale;
  float maxRadius = min(width, height) * 0.40;


  // numero particelle
  int maxParticles = int(map(currentNote, 21, 108, 300, 1500));
  int particles = int(maxParticles * visualEnergy * (0.3 + 0.7 * lpf));

  stroke(255);
  strokeWeight(2);
  noFill();

  if (visualEnergy < 0.1) {
    stroke(255);
    strokeWeight(3);
  
    float coreRadius = 10 + 15 * visualEnergy;
    int coreParticles = int(120 + 180 * visualEnergy);
  
    float t = frameCount * 0.01;
  
    for (int i = 0; i < coreParticles; i++) {
  
      float lat = latitudes[i];
      float lon = longitudes[i];
  
      // base sfera
      float x = sin(lat) * cos(lon);
      float y = cos(lat);
      float z = sin(lat) * sin(lon);
  
      // piccolo movimento organico
      float noiseVal = noise(
        x * 2 + t,
        y * 2,
        z * 2
      );
  
      float offset = map(noiseVal, 0, 1, -3, 3);
      
      float r = coreRadius + offset;
  
      float px = x * r;
      float py = y * r;
      float pz = z * r;
  
      point(px, py, pz);
    }

  return;
}

  for (int i = 0; i < particles; i++) {

    // distribuzione sferica uniforme
    float lat = latitudes[i];
    float lon = longitudes[i];
    // coordinate base sfera
    float x = sin(lat) * cos(lon);
    float y = cos(lat);
    float z = sin(lat) * sin(lon);

    // -------- ROTAZIONE LOCALE (orbita) --------

    float orbitSpeed = 0.1 + flanger * 4.0;
    float orbitRadius = 5 + chorus * 20;

    float t = frameCount * 0.002; 
    float angle = t * orbitSpeed + i;

    float orbitX = cos(angle) * orbitRadius;
    float orbitY = sin(angle) * orbitRadius;

    // -------- DISTORSIONE --------

    float noiseVal = noise(
      x * 2 + frameCount * 0.01,
      y * 2,
      z * 2
    );

    // DISTORSIONE (caotica)
    float distortionOffset = map(noiseVal, 0, 1, -180, 180) * distortion;
    distortionOffset *= (1.0 - lpf);
    
    // FLANGER (onda)
    float wave = sin(frameCount * 0.05 + i * 0.2);
    float flangerOffset = wave * 30 * flanger;
    
    // CHORUS (scala)
    float scale = 1.0 + chorus * 0.1;
    
    // RAGGIO FINALE
    float rRaw = (baseRadius + distortionOffset + flangerOffset) * scale;

    // compressione morbida invece di clamp duro
    float r = rRaw;
    if (r > maxRadius) {
      float excess = r - maxRadius;
      r = maxRadius + excess * 0.3;  // comprime oltre il limite
    }

    // posizione finale
    float px = x * r + orbitX;
    float py = y * r + orbitY;
    float pz = z * r;
    
    // posizione base PULITA (senza jitter)
    float cleanX = px;
    float cleanY = py;
    float cleanZ = pz;
    
    float distCol = smoothDist;
    float flangCol = leftThumb;
    float chorCol = chorus;
    
    float rCol = 120 + 160 * distCol;   // distorsione - rosso
    float gCol = 100 + 140 * chorCol;   // chorus - green
    float bCol = 120 + 160 * flangCol;  // flanger - blue
    
    // bilanciamento con energia
    rCol *= colorEnergy;
    gCol *= colorEnergy;
    bCol *= colorEnergy;
    
    stroke(rCol, gCol, bCol);
    
    float jitter = distortion * 8;
    
    float jitterX = random(-jitter, jitter);
    float jitterY = random(-jitter, jitter);
    float jitterZ = random(-jitter, jitter);
    
    int layers = 1 + (chorus > 0.3 ? 1 : 0);  // max 2 voci
    float spread = chorus * 8;
    
    for (int c = 0; c < layers; c++) {
    
      float offsetAngle = c * PI;
    
      float offsetX = cos(offsetAngle) * spread;
      float offsetY = sin(offsetAngle) * spread;
    
      float finalX = cleanX + offsetX;
      float finalY = cleanY + offsetY;
      float finalZ = cleanZ;
    
      if (c == 0) {
        finalX += jitterX;
        finalY += jitterY;
        finalZ += jitterZ;
      }
    
      point(finalX, finalY, finalZ);
    }
  }
}
