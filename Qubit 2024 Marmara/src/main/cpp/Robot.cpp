#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <frc/motorcontrol/VictorSP.h>
#include <frc/Timer.h>
#include <frc/PS4Controller.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <cameraserver/CameraServer.h>  



bool brake;

int shoot;
int intake;



class Robot : public frc::TimedRobot {

 public:

  void RobotInit() override {
    wpi::SendableRegistry::AddChild(&m_robotDrive, &m_frontLeft);
    wpi::SendableRegistry::AddChild(&m_robotDrive, &m_rearLeft);
    wpi::SendableRegistry::AddChild(&m_robotDrive, &m_frontRight);
    wpi::SendableRegistry::AddChild(&m_robotDrive, &m_rearRight);
    

    greyInTake2.SetInverted(true);
    Arm2.SetInverted(false);
    greyInTake.SetInverted(true);
    m_frontLeft.SetInverted(true);
    m_rearLeft.SetInverted(true);
    greyInTake2.SetInverted(true);
    greenInTake.SetInverted(true);
  }

  void brakes(){
    Arm.Set(0.05);
    Arm2.Set(0.0625);
  }

  void RobotPeriodic() override{
    frc::SmartDashboard::PutBoolean("Brakes",brake);
    frc::SmartDashboard::PutNumber("Shooters", shoot);
    frc::SmartDashboard::PutNumber("inTakers", intake);
  }

  void DisabledPeriodic() override {
      Arm.Set(0.065); //Controlling the Arm
      Arm2.Set(0.075);
  }

  void TeleopPeriodic() override {

    m_robotDrive.DriveCartesian(JoyDrv.GetRawAxis(1)/*Ileri-Geri*/, -JoyDrv.GetRawAxis(0)/*Sağ-Sol*/,
                                -JoyDrv.GetRawAxis(4)*0.5/*Donuş*/);

  if(JoyDrv.GetRawButton(2)==true){ //Shooting the note
      greenInTake.Set(-1);
      intake = 1;
   }
    else if(JoyDrv.GetRawButton(3)==true) {
      greyInTake.Set(1);
      greyInTake2.Set(1);
      shoot = -1;
  }
  
  else if(JoyDrv.GetRawButton(1)){
    greenInTake.Set(1);
    intake = -1;
  }

  else if(JoyDrv.GetRawButton(4)){ 
      greyInTake.Set(-1);
      greyInTake2.Set(-1);
      shoot = 1;
  }

  else if(JoyDrv.GetRawButton(10)) {
    greenInTake.Set(-0.5);
    greyInTake2.Set(-0.5);
    greyInTake.Set(-0.5);
  }

  else{
      greenInTake.Set(0);
      greyInTake.Set(0);
      greyInTake2.Set(0);
      intake = 0;
      shoot= 0;
  }
  

        if(JoyDrv.GetRawButton(6)){
        Arm.Set(1*0.1); //Controlling the Arm
        Arm2.Set(1*0.25);
        brake = false;
      }

      else if(JoyDrv.GetRawButton(5)){
        Arm.Set(-1*0.2); //Controlling the Arm
        Arm2.Set(-1*0.25);
        brake = false;
      }

      else if(JoyDrv.GetRawButtonPressed(5) == false && JoyDrv.GetRawButtonPressed(6) == false){
        brake=true;
        Arm.Set(0.065); //Controlling the Arm
        Arm2.Set(0.075);
      }
  }

  void AutonomousInit() override{
    timer.Reset();
    timer.Start();
  }

  void AutonomousPeriodic() override {
    if(timer.Get() < 2000_ms && timer.Get()> 0_s){ //Kol iner ve shooterlar çalışır
      Arm.Set(-1*0.3);
      Arm2.Set(-1*0.35);
      greyInTake.Set(-1);
      greyInTake2.Set(-1);
    }
    
    else if(timer.Get() > 2600_ms && timer.Get() < 4.3_s){//halka ateşlenir
      greyInTake.Set(-1);
      greyInTake2.Set(-1);
      greenInTake.Set(-1);
      
      brakes();
    }

    else if(timer.Get() > 4.3_s && timer.Get() < 5.7_s){//Kol iner
      Arm.Set(-1*0.3);
      Arm2.Set(-1*0.25);
      greyInTake.Set(-1);
      greyInTake2.Set(-1);
    }
    
    else if (timer.Get() > 5.7_s && timer.Get() < 6.6_s){//şase geri
      m_robotDrive.DriveCartesian(0.4, 0, 0);
      Arm.Set(0);
      Arm2.Set(0);
      greyInTake2.Set(0);
      greyInTake.Set(0);
      brakes();
    }

    else if (timer.Get() > 6.6_s && timer.Get() < 6.9_s){//alış
      greenInTake.Set(-1);
/*       Arm.Set(1*0.2);
      Arm2.Set(1*0.25); */
    }

    else if (timer.Get() > 6.9_s && timer.Get() < 8.2_s){ //Şase ileri
      Arm.Set(1*0.2);
      Arm2.Set(1*0.25);
      greenInTake.Set(0);
    }

    else if (timer.Get() > 10.2_s && timer.Get() < 11.2_s){//Kol yukarı
      m_robotDrive.DriveCartesian(-0.4, -0.0, 0);
      Arm.Set(0);
      Arm2.Set(0);
      greenInTake.Set(0);
      brakes();
    }

    else if(timer.Get() > 11.2_s && timer.Get() < 12.2_s){ //charge
      greyInTake.Set(-1);
      greyInTake2.Set(-1);
      brakes();
    }
    
    else if(timer.Get() > 12.2_s && timer.Get() < 13.3_s){//Atış

      greenInTake.Set(-1);
      brakes();
    }

    else if(timer.Get() > 13.3_s && timer.Get() < 15_s){//Çizgiden çıkış
      m_robotDrive.DriveCartesian(0.5, 0, 0);

    }
    else{
      m_robotDrive.DriveCartesian(0, 0, 0);
      greenInTake.Set(0);
      greyInTake2.Set(0);
      greyInTake.Set(0);
      brakes();
    }
  }

 private:
  frc::Timer timer;

  static constexpr int kFrontLeftChannel = 1;
  static constexpr int kRearLeftChannel = 3;
  static constexpr int kFrontRightChannel = 0;
  static constexpr int kRearRightChannel = 2;

  static constexpr int kJoystickChannel = 0;

  frc::VictorSP m_frontLeft{kFrontLeftChannel};
  frc::VictorSP m_rearLeft{kRearLeftChannel};
  frc::VictorSP m_frontRight{kFrontRightChannel};
  frc::VictorSP m_rearRight{kRearRightChannel};
  frc::MecanumDrive m_robotDrive{
      [&](double output) { m_frontLeft.Set(output); },
      [&](double output) { m_rearLeft.Set(output); },
      [&](double output) { m_frontRight.Set(output); },
      [&](double output) { m_rearRight.Set(output); }
      };
      
  frc::VictorSP Arm{4};
  frc::PWMSparkMax greenInTake{8};
  frc::VictorSP greyInTake{7};
  frc::VictorSP greyInTake2{6};
  frc::PWMSparkMax Arm2{5};

  frc::Joystick JoyDrv{0};
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
