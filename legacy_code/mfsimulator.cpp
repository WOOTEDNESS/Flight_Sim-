
#include "mfsimulator.h"
#include "stdio.h"

MFSimulator::MFSimulator(void)
{
		_failed = true;

#ifdef _WIN32
		_windowHandle = NULL;
		_mdiHandle = NULL;
		_subWindowHandle = NULL;
		_dialogHandle = NULL;
		_buttonRaiseHandle = NULL;
		_buttonLowerHandle = NULL;
		_buttonStopRisingHandle = NULL;
		_buttonStartHandle = NULL;
		_buttonRunHandle = NULL;
		_buttonStopHandle = NULL;
		_buttonFreezeHandle = NULL;
		_sliderRollHandle = NULL;
		_sliderPitchHandle = NULL;
		_staticRollHandle = NULL;
		_staticPitchHandle = NULL;
		_buttonLeftRaisedHandle = NULL;
		_buttonRightRaisedHandle = NULL;
		_buttonLoweredHandle = NULL;
		_buttonEmergencyStopHandle = NULL;
		_buttonCanopyOpenHandle = NULL;
		_listInfoBox = NULL;
#endif
		
		_currentPitch = 0.0;
		_currentRoll = 0.0;
		_lastSetPitch = 0;
		_lastSetRoll = 0;
		_setPitch = 0;
		_setRoll = 0;

		_leftRaised = false;
		_rightRaised = false;
		_canopyOpen = false;
		_emergencyStop = false;

		_running = false;
}


MFSimulator::~MFSimulator(void)
{
}

void MFSimulator::initialize()
{
#ifdef _WIN32
	if(_windowHandle = FindWindowA(NULL, "MFMotion Test Client"))
	{
		if(_mdiHandle = GetWindow(_windowHandle, 5))
		{
			if(_subWindowHandle = GetWindow(_mdiHandle, 5))
			{
				if(_dialogHandle = GetWindow(_subWindowHandle, 5))
				{
					_buttonRaiseHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Raise");
					_buttonLowerHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Lower");
					_buttonRunHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Run");
					_buttonStartHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Start");

					_buttonLeftRaisedHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Left Raised");
					_buttonRightRaisedHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Right Raised");
					_buttonLoweredHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Lowered");
					_buttonEmergencyStopHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Emergency Stop");
					_buttonCanopyOpenHandle = FindWindowExA(_dialogHandle, NULL, "Button", "Canopy Open");

					_sliderRollHandle = FindWindowExA(_dialogHandle, NULL, "msctls_trackbar32", "Slider1");
					_sliderPitchHandle = FindWindowExA(_dialogHandle, NULL, "msctls_trackbar32", "Slider2");
				}
			}
		}
	}

	if(	_windowHandle && 
		_mdiHandle && 
		_subWindowHandle && 
		_dialogHandle && 
		_sliderRollHandle && 
		_sliderPitchHandle && 
		_buttonLeftRaisedHandle &&
		_buttonRightRaisedHandle &&
		_buttonLoweredHandle &&
		_buttonEmergencyStopHandle &&
		_buttonCanopyOpenHandle &&
		_buttonRaiseHandle &&
		_buttonLowerHandle &&
		_buttonRunHandle &&
		_buttonStartHandle)
		_failed = false;

	else
		_failed = true;

	printf("_windowHandle = 0x%x\n", _windowHandle);
	printf("_mdiHandle = 0x%x\n", _mdiHandle);
	printf("_subWindowHandle = 0x%x\n", _subWindowHandle);
	printf("_dialogHandle = 0x%x\n", _dialogHandle);
	printf("_buttonRaiseHandle = 0x%x\n", _buttonRaiseHandle);
	printf("_buttonLowerHandle = 0x%x\n", _buttonLowerHandle);
	printf("_buttonRunHandle = 0x%x\n", _buttonRunHandle);
	printf("_buttonStartHandle = 0x%x\n", _buttonStartHandle);

	printf("_buttonLeftRaisedHandle = 0x%x\n", _buttonLeftRaisedHandle);
	printf("_buttonRightRaisedHandle = 0x%x\n", _buttonRightRaisedHandle);
	printf("_buttonEmergencyStopHandle = 0x%x\n", _buttonEmergencyStopHandle);
	printf("_buttonCanopyOpenHandle = 0x%x\n", _buttonCanopyOpenHandle);

	printf("_sliderRollHandle = 0x%x\n", _sliderRollHandle);
	printf("_sliderPitchHandle = 0x%x\n", _sliderPitchHandle);
#endif
}

void MFSimulator::update()
{
	if(_failed)
		return;

#ifdef _WIN32
	if(SendMessage(_buttonLeftRaisedHandle, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
		_leftRaised = true;

	else
		_leftRaised = false;

	if(SendMessage(_buttonRightRaisedHandle, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
		_rightRaised = true;

	else
		_rightRaised = false;

	if(SendMessage(_buttonEmergencyStopHandle, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
		_emergencyStop = true;

	else
		_emergencyStop = false;

	if(SendMessage(_buttonCanopyOpenHandle, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
		_canopyOpen = true;

	else
		_canopyOpen = false;

	if(SendMessage(_buttonLoweredHandle, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
		_lowered = true;

	else
		_lowered = false;

	//If the system is currently in emergency stop mode, then you should try to feed movement data into it.
	if(_emergencyStop)
		return;

	//if(_lastSetPitch != _setPitch)
	{
		PostMessage(_sliderPitchHandle, TBM_SETPOS, true, _setPitch);

		PostMessage(_dialogHandle, WM_VSCROLL, MAKEWPARAM(TB_THUMBPOSITION, _setPitch), (LPARAM)_sliderPitchHandle);
		PostMessage(_dialogHandle, WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, _setPitch), (LPARAM)_sliderPitchHandle);

		//PostMessage(_sliderPitchHandle,WM_LBUTTONDOWN,0,0);	
		//PostMessage(_sliderPitchHandle,WM_LBUTTONUP,0,0);

		PostMessage(_sliderPitchHandle,NM_RELEASEDCAPTURE,0,0);

		_lastSetPitch = _setPitch;
	}

	//if(_lastSetRoll != _setRoll)
	{
		PostMessage(_sliderRollHandle, TBM_SETPOS, true, _setRoll);

		PostMessage(_dialogHandle, WM_HSCROLL, MAKEWPARAM(TB_THUMBPOSITION, _setRoll), (LPARAM)_sliderRollHandle);
		PostMessage(_dialogHandle, WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, _setRoll), (LPARAM)_sliderRollHandle);

		//PostMessage(_sliderRollHandle,WM_LBUTTONDOWN,0,0);	
		//PostMessage(_sliderRollHandle,WM_LBUTTONUP,0,0);

		PostMessage(_sliderRollHandle,NM_RELEASEDCAPTURE,0,0);

		_lastSetRoll = _setRoll;
	}
#endif
}

void MFSimulator::raise()
{
}

void MFSimulator::lower()
{
}

void MFSimulator::start()
{
}

void MFSimulator::stop()
{
}

void MFSimulator::setPitch(float fPitch)
{
	_setPitch = (int)fPitch;

	while(_setPitch >= 360)
	{
		_setPitch -= 360;
	}

	while(_setPitch <= -360)
	{
		_setPitch += 360;
	}
}

void MFSimulator::setPitch(int iPitch)
{
	_setPitch = (int)iPitch;

	while(_setPitch >= 360)
	{
		_setPitch -= 360;
	}

	while(_setPitch <= -360)
	{
		_setPitch += 360;
	}
}

void MFSimulator::setRoll(float fRoll)
{
	_setRoll = (int)fRoll;

	while(_setRoll >= 360)
	{
		_setRoll -= 360;
	}

	while(_setRoll <= -360)
	{
		_setRoll += 360;
	}
}

void MFSimulator::setRoll(int iRoll)
{
	_setRoll = (int)iRoll;

	while(_setRoll >= 360)
	{
		_setRoll -= 360;
	}

	while(_setRoll <= -360)
	{
		_setRoll += 360;
	}
}

int MFSimulator::getPitch()
{
	return _setPitch;
}

int MFSimulator::getRoll()
{
	return _setRoll;
}

float MFSimulator::getCurrentPitch()
{
	return 0.0f;
}

float MFSimulator::getCurrentRoll()
{
	return 0.0f;
}

bool MFSimulator::isLeftRaised()
{
	return _leftRaised;
}

bool MFSimulator::isRightRaised()
{
	return _rightRaised;
}

bool MFSimulator::isRaised()
{
	return _rightRaised && _leftRaised;
}

bool MFSimulator::isLowered()
{
	return _lowered;
}

bool MFSimulator::isCanopyOpen()
{
	return _canopyOpen;
}

bool MFSimulator::isEmergencyStop()
{
	return _emergencyStop;
}