#pragma once

/**
This class attempts to take control of the MFClient Program
by grabbing handles to all the event in the first motion
control window it can find and then allows the user of this 
class to control pitch, and roll.

It also uses the static information and the info box to
determine if other events from a user at the terminal have
occured, also can keep track of safty systems.
*/

#ifdef _WIN32
#include "windows.h"
#include "Commctrl.h"
#endif

class MFSimulator
{
	private:
#ifdef _WIN32
		HWND _windowHandle;
		HWND _mdiHandle;
		HWND _subWindowHandle;
		HWND _dialogHandle;
		HWND _buttonRaiseHandle;
		HWND _buttonLowerHandle;
		HWND _buttonStopRisingHandle;
		HWND _buttonStartHandle;
		HWND _buttonRunHandle;
		HWND _buttonStopHandle;
		HWND _buttonFreezeHandle;
		HWND _sliderRollHandle;
		HWND _sliderPitchHandle;
		HWND _staticRollHandle;
		HWND _staticPitchHandle;
		HWND _buttonLeftRaisedHandle;
		HWND _buttonRightRaisedHandle;
		HWND _buttonLoweredHandle;
		HWND _buttonEmergencyStopHandle;
		HWND _buttonCanopyOpenHandle;
		HWND _listInfoBox;
#endif
		
		bool _failed; 

		float _currentPitch;
		float _currentRoll;
		int _lastSetPitch;
		int _lastSetRoll;
		int _setPitch;
		int _setRoll;

		bool _emergencyStop;
		bool _canopyOpen;
		bool _leftRaised;
		bool _rightRaised;
		bool _lowered;

		bool _running;

		double _lastUpdateTime;

	public:
		MFSimulator(void);
		~MFSimulator(void);

		void initialize();
		void update();

		void raise();
		void lower();
		void start();
		void stop();

		void setPitch(float fPitch);
		void setPitch(int iPitch);
		void setRoll(float fRoll);
		void setRoll(int iRoll);

		int getPitch();
		int getRoll();
		float getCurrentPitch();
		float getCurrentRoll();

		bool isLeftRaised();
		bool isRightRaised();
		bool isRaised();
		bool isLowered();
		bool isCanopyOpen();
		bool isEmergencyStop();
};