// Fixed drawCar function
void Showroom::drawCar(float x, float y, float z, float angle, int type) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0, 1, 0);
    
    if(type == 0 && shelbyModel) {
        shelbyModel->draw();
    } else if(type == 1 && mercModel) {
        mercModel->draw();
    } else {
        glScalef(1.5f, 1.5f, 1.5f);
        if(type == 0) glColor3f(1, 0.1f, 0.1f);
        else if(type == 1) glColor3f(0.2f, 0.4f, 0.9f);
        else glColor3f(0.1f, 0.1f, 0.1f);
        glCallList(carDisplayList);
    }
    
    glPopMatrix();
}

// Fixed toggleEngine
void toggleEngine() {
    engineOn = !engineOn;
    if(engineOn) {
        mciSendStringA("close carengine", NULL, 0, NULL);
        mciSendStringA("open \"sounds\\car-engine.wav\" type waveaudio alias carengine", NULL, 0, NULL);
        mciSendStringA("play carengine repeat", NULL, 0, NULL);
    } else {
        mciSendStringA("stop carengine", NULL, 0, NULL);
        mciSendStringA("close carengine", NULL, 0, NULL);
    }
}
