#pragma once

namespace demo {
	class Demo {

	public:
		Demo() {}
		virtual	~Demo() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}

	protected:
	};
}