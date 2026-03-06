
#include "QApp.h"

//#ifdef _WIN32
//// 告诉 NVIDIA 驱动程序，此程序希望使用高性能显卡
//extern "C" {
//	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//}
//
//// 告诉 AMD 驱动程序，此程序希望使用高性能显卡
//extern "C" {
//	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
//}
//#endif

QRunnable bg_color_runnable();
QRunnable point_runnable();
QRunnable cube_runnable();
QRunnable multi_cube_runnable();
QRunnable multi_model_runnable();

int demo_main(QRunnable runnable ) {
	QApp app(runnable);
	if (int ret = app.Init(); ret != 0) {
		return ret;
	}
	return app.Run();
}

int main() {
	//return demo_main(bg_color_runnable());
	//return demo_main(point_runnable());
	//return demo_main(cube_runnable());
	//return demo_main(multi_cube_runnable());
	return demo_main(multi_model_runnable());
}

