#pragma once
#include <memory>
#include <chrono>
#include "Scene.h"
#include <array>
#include "ECSManager.h"
#include "ThreadManager.h"
#include "InputManager_DX.h"
#include "InputManager_GL.h"
#include "GUIManager.h"
#include "ResourceManager.h"

class SceneManager
{
private:
	//Managers
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<ThreadManager> mThreadManager = ThreadManager::Instance();
	std::shared_ptr<GUIManager> mGUIManager = GUIManager::Instance();
	std::shared_ptr<ResourceManager> mResourceManager = ResourceManager::Instance();
#ifdef  DIRECTX
	std::shared_ptr<InputManager_DX> mInputManager = InputManager_DX::Instance();
#elif OPENGL
	std::shared_ptr<InputManager_GL> mInputManager = InputManager_GL::Instance();
#endif

	//Timing variables
	std::chrono::nanoseconds mDeltaTime;
	std::chrono::high_resolution_clock::time_point mCurrentTime;
	std::chrono::high_resolution_clock::time_point mPreviousTime;
	std::chrono::high_resolution_clock::time_point mStartTime;
	int mFps = 0;
	double mAverageDeltaTime = 0;
	std::array<double, 50> mLast50Frames;
	bool mDeltaTimeSet = false;

	//Active scene
	std::shared_ptr<Scene> mScene;

	float mWindowWidth;
	float mWindowHeight;

	//Private constructor for singleton pattern
	SceneManager();

public:
	~SceneManager();

	void Update();

	//Timing functions
	const double DeltaTime() const;
	const double Time() const;
	const int& Fps() const;

	//Window width/height functions
	void SetWindowWidthHeight(const float& pWidth, const float& pHeight);
	const float& WindowWidth() const;
	const float& WindowHeight() const;

	const void Pause(bool mIsPaused);

	/// <summary>
	/// Loads scene of given type T
	/// </summary>
	template <class T>
	const void LoadScene()
	{
		//Unload previous scene
		if (mScene)
		{
			mScene->OnUnload();
		}

		//Create new scene of type T
		mScene = std::make_shared<T>();

		//Load new scene
		mScene->OnLoad();

		//Set start time of scene
		mStartTime = std::chrono::high_resolution_clock::now();


	};

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	SceneManager(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	static std::shared_ptr<SceneManager> Instance();
};
