#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>


using namespace std;

// ID untuk texture
GLuint texture_Earth_ID;
GLuint texture_Snow_ID;
GLuint texture_Batang_ID;
GLuint texture_Daun_ID;

// variabel untuk pergerakan mouse
float cameraYaw = 0.0f; // Rotasi horizontal
float cameraPitch = 0.0f; // Rotasi vertikal
float lastMouseX, lastMouseY;
bool firstMouse = true;
bool hidden = false;

// Posisi kamera
float cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 5.0f;
bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;
GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};

// --------------------------------------------LIGHTING----------------------------
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightAmb[] = {0.2f, 0.2f, 0.2f, 0.0f};
    GLfloat lightDiff[] = {0.8f, 0.8f, 0.8f, 0.0f};
    GLfloat lightSpec[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}
//-----------------------------------------------MOUNTAIN----------------------------------------
void gunung(void)
{
	glColor3f(0.816, 0.851, 0.871);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 3000.0); // Geser ke kiri
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glScalef(3500.5, 3000.5, 1200.0);     // Ukuran pertama
	glutSolidCone(0.5, 3.0, 100, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-950.0, 0.0, 2750.0); // Geser ke kiri
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glScalef(2700.5, 2700.5, 900.0);     // Ukuran pertama
	glutSolidCone(0.5, 3.0, 100, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(950.0, 0.0, 2800.0); // Geser ke kiri
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glScalef(2660.5, 2660.5, 850.0);     // Ukuran pertama
	glutSolidCone(0.5, 3.0, 100, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1500.0, 0.0, 2500.0); // Geser ke kiri
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glScalef(2350.5, 2240.5, 700.0);     // Ukuran pertama
	glutSolidCone(0.5, 3.0, 100, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1500.0, 0.0, 2500.0); // Geser ke kiri
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glScalef(2400.5, 2240.5, 730.0);     // Ukuran pertama
	glutSolidCone(0.5, 3.0, 100, 1);
	glPopMatrix();


	glFlush();
}


//-------------------------------------------------------IGLOO----------------------------------------------------

std::vector<float> vertices;
std::vector<unsigned int> indices;

// Function to load .obj file
bool loadOBJ(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") { // Vertex
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        } else if (type == "f") { // Face
            unsigned int index;
            while (iss >> index) {
                indices.push_back(index - 1); // Convert to 0-based index
            }
        }
    }

    std::cout << "Loaded " << vertices.size() / 3 << " vertices and " << indices.size() / 3 << " faces." << std::endl;
    return true;
}

// Function to draw OBJ model
void drawOBJ() {
    glColor3f(1.0, 1.0, 1.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
}


//----------------------------------------------------SALJU---------------------------------------------------------------


// define untuk efek hujan salju
#define JUMLAH_SALJU 15000
#define UKURAN_SALJU 1.5
#define UKURAN_AREA 4000.0f
#define KETINGGIAN_LANGIT 4000.0f  // Salju akan jatuh dari ketinggian ini

// Variabel global untuk posisi salju
float posisiX[JUMLAH_SALJU];
float posisiY[JUMLAH_SALJU];
float posisiZ[JUMLAH_SALJU];
float kecepatan[JUMLAH_SALJU];
float rotasiX[JUMLAH_SALJU];
float rotasiY[JUMLAH_SALJU];
float rotasiZ[JUMLAH_SALJU];
float kecepatanPutar[JUMLAH_SALJU];


 // Membuat array posisi dan kecepatan salju
// Menghasilkan posisi acak untuk setiap butiran salju
void inisialisasiSalju() {
    srand(time(NULL));
    for(int i = 0; i < JUMLAH_SALJU; i++) {
        posisiX[i] = ((float)rand()/RAND_MAX * 2 - 1) * UKURAN_AREA;
        posisiY[i] = ((float)rand()/RAND_MAX) * UKURAN_AREA * 2;
        posisiZ[i] = ((float)rand()/RAND_MAX * 2 - 1) * UKURAN_AREA;
        kecepatan[i] = 2.1 + ((float)rand()/RAND_MAX) * 1.2;
        rotasiX[i] = ((float)rand()/RAND_MAX) * 360;
        rotasiY[i] = ((float)rand()/RAND_MAX) * 360;
        rotasiZ[i] = ((float)rand()/RAND_MAX) * 360;
        kecepatanPutar[i] = ((float)rand()/RAND_MAX) * 2.0 + 0.5;
    }
}

// Menggambar setiap butiran salju
void gambarKepinganSalju(float ukuran) {
    glDisable(GL_TEXTURE_2D); // Nonaktifkan tekstur
    glBegin(GL_LINES);

    // Sumbu X
    glVertex3f(-ukuran, 0.0f, 0.0f);
    glVertex3f(ukuran, 0.0f, 0.0f);

    // Sumbu Y
    glVertex3f(0.0f, -ukuran, 0.0f);
    glVertex3f(0.0f, ukuran, 0.0f);

    // Sumbu Z
    glVertex3f(0.0f, 0.0f, -ukuran);
    glVertex3f(0.0f, 0.0f, ukuran);

    // Diagonal XY
    glVertex3f(-ukuran/1.4f, -ukuran/1.4f, 0.0f);
    glVertex3f(ukuran/1.4f, ukuran/1.4f, 0.0f);
    glVertex3f(-ukuran/1.4f, ukuran/1.4f, 0.0f);
    glVertex3f(ukuran/1.4f, -ukuran/1.4f, 0.0f);

    // Diagonal YZ
    glVertex3f(0.0f, -ukuran/1.4f, -ukuran/1.4f);
    glVertex3f(0.0f, ukuran/1.4f, ukuran/1.4f);
    glVertex3f(0.0f, -ukuran/1.4f, ukuran/1.4f);
    glVertex3f(0.0f, ukuran/1.4f, -ukuran/1.4f);

    // Diagonal XZ
    glVertex3f(-ukuran/1.4f, 0.0f, -ukuran/1.4f);
    glVertex3f(ukuran/1.4f, 0.0f, ukuran/1.4f);
    glVertex3f(-ukuran/1.4f, 0.0f, ukuran/1.4f);
    glVertex3f(ukuran/1.4f, 0.0f, -ukuran/1.4f);

    glEnd();
    glEnable(GL_TEXTURE_2D); // Aktifkan kembali tekstur
}

//------------------------------------------------AREA-------------------------------------------------
void createObject()
{
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);
    gluSphere(object, 5000 , 100, 100);
}
void snowFloor() {
    glPushMatrix();
    // Transformasi lantai (letakkan di bawah layar)
    glTranslatef(0.0f, -10.0f, 0.0f); // Posisikan lantai lebih rendah
    // Aktifkan tekstur salju
    glBindTexture(GL_TEXTURE_2D, texture_Snow_ID);
    // Buat kubus besar sebagai lantai 3D
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // Abaikan warna jika tekstur digunakan

    // Bagian atas lantai (menghadap ke atas)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10000.0f, 0.0f, 10000.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10000.0f, 0.0f, 10000.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10000.0f, 0.0f, -10000.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10000.0f, 0.0f, -10000.0f);

    // Bagian bawah lantai (menghadap ke bawah)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10000.0f, -10.0f, 10000.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10000.0f, -10.0f, 10000.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10000.0f, -10.0f, -10000.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10000.0f, -10.0f, -10000.0f);

    // Sisi-sisi lantai untuk ketebalan
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10000.0f, 0.0f, 10000.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10000.0f, -10.0f, 10000.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10000.0f, -10.0f, -10000.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10000.0f, 0.0f, -10000.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(10000.0f, 0.0f, 10000.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10000.0f, -10.0f, 10000.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10000.0f, -10.0f, -10000.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10000.0f, 0.0f, -10000.0f);

    glPopMatrix();
    glEnd();
}


//----------------------------------------------POHON-----------------------------------------------


/// Fungsi untuk menggambar batang pohon
void batang(GLfloat radius, GLfloat height) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    // Warna batang cokelat
    glColor3f(0.55f, 0.27f, 0.07f);
    
    // Batang silinder
    gluCylinder(quadric, radius, radius, height, 32, 32);
    
    // Tutup bagian bawah
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(quadric, 0, radius, 32, 1);
    glPopMatrix();
    
    // Tutup bagian atas
    glPushMatrix();
    glTranslatef(0, 0, height);
    gluDisk(quadric, 0, radius, 32, 1);
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}

// Fungsi untuk menggambar daun pohon
void daun(GLfloat baseRadius, GLfloat height) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    // Warna daun hijau
    glColor3f(0.0f, 0.5f, 0.0f);
    
    // Kerucut daun
    gluCylinder(quadric, baseRadius, 0.0f, height, 32, 32);
    
    // Tutup bagian bawah
    glPushMatrix();
    gluDisk(quadric, 0, baseRadius, 32, 1);
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}

// Fungsi untuk menggambar pohon
void pohon() {
    // pohon 1
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();


    // pohon 2
    glPushMatrix();
    glTranslatef(175.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 3
    glPushMatrix();
    glTranslatef(-175.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 4
    glPushMatrix();
    glTranslatef(100.0f, 0.0f, -4200.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 5
    glPushMatrix();
    glTranslatef(-450.0f, 0.0f, -3800.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 6
    glPushMatrix();
    glTranslatef(-250.0f, 0.0f, -3800.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 7
    glPushMatrix();
    glTranslatef(250.0f, 0.0f, -3800.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

     // pohon 8
    glPushMatrix();
    glTranslatef(-450.0f, 0.0f, -3600.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();


     // pohon 9
    glPushMatrix();
    glTranslatef(400.0f, 0.0f, -3550.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();


     // pohon 10
    glPushMatrix();
    glTranslatef(355.0f, 0.0f, -3800.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // pohon 11
    glPushMatrix();
    glTranslatef(-600.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
    // pohon 11
    glPushMatrix();
    glTranslatef(600.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 64.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 160.0f);
    
    glTranslatef(0.0f, 0.0f, 80.0f);
    daun(96.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 64.0f);
    daun(72.0f, 128.0f);
    
    glTranslatef(0.0f, 0.0f, 56.0f);
    daun(48.0f, 56.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

     // pohon 12
    glPushMatrix();
    glTranslatef(-600.0f, 0.0f, -4000.0f);  // Posisi awal pohon
    glPushMatrix();
    // Batang pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -64.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    batang(16.f, 128.0f);
    glPopMatrix();

    // Daun pohon
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);  // Rotasi untuk posisi vertikal
    daun(120.0f, 320.0f);
    
    glTranslatef(0.0f, 0.0f, 160.0f);
    daun(96.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 128.0f);
    daun(72.0f, 256.0f);
    
    glTranslatef(0.0f, 0.0f, 112.0f);
    daun(48.0f, 112.0f);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

}


//---------------------------------------------BATAS PINUS----------------------------


// Fungsi untuk menggambar garis kartesius
void Cartecius()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(-4000.0, 0.0, 0.0);
	glVertex3f(4000.0, 0.0, 0.0);
	
	glVertex3f(0.0, -4000.0, 0.0);	
	glVertex3f(0.0, 4000.0, 0.0);
	
	glVertex3f(0.0, 0.0, -4000.0);	
	glVertex3f(0.0, 0.0, 4000.0);	
	glEnd();
}

void updateCamera()
{
    // Update posisi kamera berdasarkan input
    float cameraSpeed = 50.0f; // Kecepatan pergerakan kamera
    if (moveForward) {
        cameraPosX += sinf(cameraYaw) * cameraSpeed;
        cameraPosZ -= cosf(cameraYaw) * cameraSpeed;
    }
    if (moveBackward) {
        cameraPosX -= sinf(cameraYaw) * cameraSpeed;
        cameraPosZ += cosf(cameraYaw) * cameraSpeed;
    }
    if (moveLeft) {
        cameraPosX -= cosf(cameraYaw) * cameraSpeed;
        cameraPosZ -= sinf(cameraYaw) * cameraSpeed;
    }
    if (moveRight) {
        cameraPosX += cosf(cameraYaw) * cameraSpeed;
        cameraPosZ += sinf(cameraYaw) * cameraSpeed;
    }
    // Pastikan kamera tidak terlalu rendah
    if (cameraPosY < 1.6f) cameraPosY = 1.6f; // Tinggi minimum level mata manusia
}

//----------------------------------------DISPLAY------------------------------------


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Atur kamera berdasarkan rotasi dan posisi
    updateCamera();
    float camX = cameraPosX + cosf(cameraPitch) * sinf(cameraYaw);
    float camY = cameraPosY + sinf(cameraPitch); // Arah pandangan vertikal
    float camZ = cameraPosZ + cosf(cameraPitch) * cosf(cameraYaw);
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, camX, camY, camZ, 0.0f, 1.0f, 0.0f);


    // Render lantai salju
    glPushMatrix();
    snowFloor();
    glPopMatrix();

    //BIND TExture ke Object di bawahnya, dengan ID yang sudah di simpan tadi
    glPushMatrix();
    glBindTexture( GL_TEXTURE_2D, texture_Earth_ID);
    createObject();
    glPopMatrix();

    // Gambar salju
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < JUMLAH_SALJU; i++) {
        glPushMatrix();
        glTranslatef(posisiX[i], posisiY[i], posisiZ[i]);
        glRotatef(rotasiX[i], 1.0, 0.0, 0.0);
        glRotatef(rotasiY[i], 0.0, 1.0, 0.0);
        glRotatef(rotasiZ[i], 0.0, 0.0, 1.0);
        gambarKepinganSalju(UKURAN_SALJU);
        glPopMatrix();
    }
    glPopMatrix();   

    // cartecius
    if(hidden){
    Cartecius();
    }
    
    drawOBJ();
    
    gunung();

    glPushMatrix();
    pohon();
    glTranslatef(10.0f, 0.0f, 0.0f);
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}


GLuint textureID = 0;

// Memperbarui posisi dan rotasi salju
// Membuat efek jatuh dan bergerak
void perbaruiSalju(int nilai) {
    for(int i = 0; i < JUMLAH_SALJU; i++) {
        // Update posisi
        posisiY[i] -= kecepatan[i];
        posisiX[i] += sin(posisiY[i]/30.0) * 0.1;
        posisiZ[i] += cos(posisiY[i]/30.0) * 0.1;
        
        // Update rotasi
        rotasiX[i] += kecepatanPutar[i];
        rotasiY[i] += kecepatanPutar[i];
        rotasiZ[i] += kecepatanPutar[i];
        
        // Reset jika keluar area
        if(posisiY[i] < 0) {
            posisiY[i] = UKURAN_AREA * 2;
            posisiX[i] = ((float)rand()/RAND_MAX * 2 - 1) * UKURAN_AREA;
            posisiZ[i] = ((float)rand()/RAND_MAX * 2 - 1) * UKURAN_AREA;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, perbaruiSalju, 0);
}

// Memuat tekstur menggunakan FreeImage
// Mengonversi gambar menjadi tekstur OpenGL
//penggunaan Texture ini bisa di gambarkan sebagai array
int loadTexture(const char* path) 
{
    // untuk menyimpan Data Texture di ID spesifik!
    glGenTextures(1, &textureID);
    // kode di bawah untuk Memproses Pembacaan/Penyimpanan Buffer dari Gambar
    void* imgData;
    int imgWidth;
    int imgHeight;
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);
    if (imgData) {
        printf("Texture image loaded from file %s, size %dx%d\n", path,
        imgWidth, imgHeight);
        int format;
        if ( FI_RGBA_RED == 0 )
        format = GL_RGB;
        else
        format = GL_BGR;
        glBindTexture( GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0,
        format,GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        textureID++;
        return textureID-1;
    }
    else {
    printf("Failed to get texture data from %s\n", path);
    }
    return -1;
}

void initProjection(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);//untuk meaktifkan texture di Polygon
    glMatrixMode(GL_TEXTURE);//untuk Mengubah matrik menjadi texture Rendering di OpenGL
    glMatrixMode(GL_PROJECTION);//END
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 50000.0);
    glMatrixMode(GL_MODELVIEW);
    texture_Earth_ID = loadTexture("images/aurora.jpg");
    texture_Snow_ID = loadTexture("images/landasanSalju.png");
    
}

// Kontrol pergerakan kamera (WASD)
// Kontrol ketinggian kamera
// Kontrol cahaya
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w': // Maju
            moveForward = true;
            break;
        case 's': // Mundur
            moveBackward = true;
            break;
        case 'a': // Kiri
            moveLeft = true;
            break;
        case 'd': // Kanan
            moveRight = true;
            break;
        case 'r': // Naikkan kamera
            cameraPosY += 50.0f;
            break;
        case 'f': // Turunkan kamera
            cameraPosY -= 50.0f;
            break;
        case 'h':
            hidden = !hidden;
            break;
        
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w': // Maju
            moveForward = false;
            break;
        case 's': // Mundur
            moveBackward = false;
            break;
        case 'a': // Kiri
            moveLeft = false;
            break;
        case 'd': // Kanan
            moveRight = false;
            break;
    }
}

// Mengubah arah pandang kamera
// Mengatur sensitivitas gerakan
void mouseMotion(int x, int y)
{
        if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    // Tingkatkan sensitivitas gerakan
    const float sensitivity = 0.0025f; 
    float xOffset = (x - lastMouseX) * sensitivity;
    float yOffset = (lastMouseY - y) * sensitivity; // Y terbalik
    lastMouseX = x;
    lastMouseY = y;
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    // Batasi pitch untuk mencegah "flip" kamera
    if (cameraPitch > 1.5f) cameraPitch = 1.5f;
    if (cameraPitch < -1.5f) cameraPitch = -1.5f;

}

// Inisialisasi GLUT
// Pengaturan window
// Registrasi fungsi callback
// Memuat model 3D
// Memulai loop utama
int main(int argc, char** argv){
 //catatan disini Kenapa RGBA bukan RGB?? karena texture meminta ALPHA sebagai bahan render
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1800, 900);
    glutInitWindowPosition(100,-50);
    glutCreateWindow("TB Kelompok 9 - Suasana di Antartika");
    glewInit();
    initProjection(); 
    setupLighting();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    inisialisasiSalju();
    
    if (!loadOBJ("model/terakhirrrr.obj")) {
        std::cerr << "Error loading OBJ file." << std::endl;
    }

    glutTimerFunc(0, perbaruiSalju, 0);
    glutMainLoop();
    return 0;
}