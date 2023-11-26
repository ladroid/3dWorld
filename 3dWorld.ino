#include <Arduboy2.h>

Arduboy2 arduboy;

const int mapWidth = 100;
const int mapHeight = 100;

float posX = 50.0, posY = 50.0; // Initial position
float dirX = -1.0, dirY = 0.0; // Initial direction vector
float planeX = 0.0, planeY = 0.66; // Camera plane

// Basic hash function for map generation
unsigned int hash(int x, int y) {
  return (x * 88339u) ^ (y * 91967u) ^ (88339u * 91967u);
}

// Function to determine if a map cell is a wall
bool isWall(int x, int y) {
  if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return true; // Boundaries
  unsigned int h = hash(x, y);
  return (h % 4 == 0); // Adjust for desired wall density
}

void setup() {
  arduboy.begin();
  arduboy.clear();
}

void loop() {
  if (!arduboy.nextFrame()) return;
  arduboy.clear();

  for (int x = 0; x < arduboy.width(); x++) {
    // Calculate ray position and direction
    double cameraX = 2 * x / double(arduboy.width()) - 1;
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;

    // Which box of the map we're in
    int mapX = int(posX);
    int mapY = int(posY);

    // Length of ray from current position to next x or y-side
    double sideDistX, sideDistY;

    // Length of ray from one x or y-side to next x or y-side
    double deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);
    double perpWallDist;

    // What direction to step in x or y-direction (+1 or -1)
    int stepX, stepY;
    int hit = 0; // Was there a wall hit?
    int side; // Was a NS or a EW wall hit?

    // Calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (posX - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // Perform DDA
    while (hit == 0) {
      // Jump to next map square, in x-direction or y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      // Check if ray has hit a wall
      if (isWall(mapX, mapY)) hit = 1;
    }

    // Calculate distance to the point of wall hit
    if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
    else           perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

    // Calculate height of wall slice to draw
    int lineHeight = (int)(arduboy.height() / perpWallDist);

    // Calculate where to draw the wall slice
    int drawStart = -lineHeight / 2 + arduboy.height() / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + arduboy.height() / 2;
    if (drawEnd >= arduboy.height()) drawEnd = arduboy.height() - 1;

    // Draw the wall slice
    // Determine rendering based on wall distance
    if (perpWallDist < 1.0) {  // Close walls are solid white
      arduboy.drawFastVLine(x, drawStart, drawEnd - drawStart, WHITE);
    } else {  // Distant walls are rendered with dithering
      for (int y = drawStart; y < drawEnd; y++) {
        if ((x + y) % 2 == 0) {
          arduboy.drawPixel(x, y, WHITE);
        }
      }
    }
  }

  // Player movement and rotation
  double moveSpeed = 0.05; // Adjust as needed
  double rotSpeed = 0.03; // Adjust as needed

  if (arduboy.pressed(UP_BUTTON)) {
    if (!isWall(int(posX + dirX * moveSpeed), int(posY)))
      posX += dirX * moveSpeed;
    if (!isWall(int(posX), int(posY + dirY * moveSpeed)))
      posY += dirY * moveSpeed;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (!isWall(int(posX - dirX * moveSpeed), int(posY)))
      posX -= dirX * moveSpeed;
    if (!isWall(int(posX), int(posY - dirY * moveSpeed)))
      posY -= dirY * moveSpeed;
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    // Rotate left
    double oldDirX = dirX;
    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
    double oldPlaneX = planeX;
    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    // Rotate right
    double oldDirX = dirX;
    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
    double oldPlaneX = planeX;
    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
  }

  arduboy.display();
}
