class MassPoint{
  private:
    float mass;
    float oomass;
    float force[2];
    float pos[2];
    float vel[2];
    float deep;
    float aclUp;
  public:
    MassPoint();
};

class Spring{
  private:
    int mass[2];
    float length;
    float stiffness;
  public:
    Spring();
    setupBox();
    boxSafeDrop();
};

MassPoint::MassPoint(){
  mass = 0;
  oomass = 0;
  force[0] = 0;
  force[1] = 0;
  pos[0] = 0;
  pos[1] = 0;
  vel[0] = 0;
  vel[1] = 0;
  deep = 0;
  aclUp = 0;
}

Spring::Spring(){
  mass[0] = 0;
  mass[1] = 0;
  length = 0;
  stiffness = 0;
}
