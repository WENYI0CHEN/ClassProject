#pragma once

namespace fw
{
	class FWCore;

	class GameCore
	{
	public:
		GameCore(FWCore* framework);
		virtual ~GameCore();

		virtual void onKey(int keyCode, bool pressed) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void draw() = 0;

	protected:
		FWCore* m_pFramework = nullptr;
	};
}
