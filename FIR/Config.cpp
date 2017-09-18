
#include"Config.hpp"


Config::Config() {

	gui = GUI(GUIStyle::Default);

	gui.setTitle(L"Config");
	gui.add(L"select", GUIButton::Create(L"ëIë"));
	gui.addln(L"fir", GUIButton::Create(L"ÉtÉBÉãÉ^ä|ÇØ"));

	gui.addln(L"sound_name", GUIText::Create(L"", 300));
	gui.add(L"play_base", GUIButton::Create(L"å≥âπåπçƒê∂"));
	gui.addln(L"stop_base", GUIButton::Create(L"å≥âπåπí‚é~"));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);

	gui.addln(L"fir_name", GUIText::Create(L"", 300));
	gui.add(L"play_fir", GUIButton::Create(L"ïœä∑âπåπçƒê∂"));
	gui.addln(L"stop_fir", GUIButton::Create(L"ïœä∑âπåπí‚é~"));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);

	gui.addln(L"degree_num", GUIText::Create(L"", 300));
	gui.add(L"degree", GUISlider::Create(1.0, 50.0, 5, 200));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);

	gui.addln(L"resolution_num", GUIText::Create(L"", 300));
	gui.addln(L"resolution", GUISlider::Create(10, 100.0, 10, 200));

	gui.add(L"normalization", GUIToggleSwitch::Create(L"îÒê≥ãKâª", L"ê≥ãKâª", false));

	a = std::vector<double>(gui.slider(L"degree")._get_valueInt(), 0);
	resolution_fir = 1.0 / (double)gui.slider(L"resolution").valueInt;
	font = Font(20);
	normalization = false;
}


void Config::update() {

	this->isReaction();
	this->setText();
	this->draw();

}


void Config::isReaction() {


	if (gui.button(L"select").pushed) {
		if (!sound_base.isEmpty())sound_base.release();
		if (!sound_fir.isEmpty())sound_fir.release();
		while (sound_base.isEmpty()) {
			Optional<String> state = Dialog::GetOpenSound();
			if (!state.has_value()) {
				break;
			}
			sound_base = Sound(state.value());
			Optional<AudioProperty> audio = Audio::GetProperty(state.value());
			sound_name = audio->title.narrow();
			if (sound_name.length() > 25) {
				sound_name = sound_name.substr(0, 20);
				sound_name += "...";
			}
		}
	}

	if (gui.button(L"fir").pushed)this->convolution();

	if (gui.button(L"play_base").pushed) {
		if (!sound_base.isEmpty())
			if (!sound_base.isPlaying())sound_base.play();
	}

	if (gui.button(L"stop_base").pushed) {
		if (!sound_base.isEmpty())
			if (sound_base.isPlaying())sound_base.stop();
	}

	if (gui.button(L"play_fir").pushed) {
		if (!sound_fir.isEmpty())
			if (!sound_fir.isPlaying())sound_fir.play();
	}

	if (gui.button(L"stop_fir").pushed) {
		if (!sound_fir.isEmpty())
			if (sound_fir.isPlaying())sound_fir.stop();
	}

	if (gui.slider(L"degree").hasChanged)a = std::vector<double>(gui.slider(L"degree")._get_valueInt(), 0);

	if (gui.slider(L"resolution").hasChanged)resolution_fir = 1.0 / (double)gui.slider(L"resolution")._get_valueInt();


	if (gui.toggleSwitch(L"normalization").hasChanged) {

		if (normalization) {
			if (abs(std::accumulate(a.begin(), a.end(), 0.0)) <= 1.0) {
				gui.toggleSwitch(L"normalization").setLeft();
				normalization = false;
			}else {
				gui.toggleSwitch(L"normalization").setRight();
				normalization = true;
			}
		}else {
			gui.toggleSwitch(L"normalization").setRight();
			normalization = true;
		}

	}
	
	if (Input::MouseL.clicked) {
		if (!gui.style.visible) {
			parameter = this->calcArea(Mouse::Pos().x, Mouse::Pos().y);
			this->setCoefficient(parameter.first, parameter.second);
		}
	}

	if (Input::MouseR.clicked) {
		if (gui.style.visible)this->gui.show(false);
		else this->gui.show(true);
	}

}


void Config::setText() {
	if (!sound_base.isEmpty())gui.text(L"sound_name").text = Format(L"âπåπñº: ", Widen(sound_name));
	if (!sound_fir.isEmpty())gui.text(L"fir_name").text = Format(L"âπåπñº: ", Widen(sound_name));
	gui.text(L"degree_num").text = Format(L"ÉtÉBÉãÉ^éüêî: ", gui.slider(L"degree")._get_valueInt());
	gui.text(L"resolution_num").text = Format(L"ï™âî\: ", 1.0 / (double)gui.slider(L"resolution")._get_valueInt());
}


std::pair<bool, int> Config::calcArea(int x, int y) {

	int index;
	double resolution;

	resolution = Window::Width() / (double)a.size();
	index = (int)(x / resolution);

	if (y > Window::Height() / 2)return std::pair<bool, int>(false, index);
	else return std::pair<bool, int>(true, index);
}

void Config::setCoefficient(bool flag, int index) {

	double resolution;
	if (flag)resolution = resolution_fir;
	else resolution = -1.0*resolution_fir;

	if (!normalization)if (abs(std::accumulate(a.begin(), a.end(), 0.0) + resolution) > 1.0)return;

	a[index] += resolution;

}



void Config::draw() {

	double resolution_x = Window::Width() / (double)a.size();
	std::string str;
	std::pair<bool, int>here;

	// block
	for (unsigned int i = 0; i < a.size(); i++) {
		RectF(i * resolution_x, Window::Height() / 2, resolution_x, (Window::Height() / 2.0)*-a[i]).draw(HSV(0.0, 1.0, 1.0));
	}

	// border
	for (unsigned int i = 0; i < a.size(); i++) {
		RectF((i + 1) * resolution_x, Window::Height(), 3, -Window::Height()).draw(HSV(0.0, 0.0, 1.0));
	}
	RectF(0.0, (Window::Height()/2)-1, Window::Width(), 3.0).draw(HSV(0.0, 0.0, 1.0));


	// now_position
	here = this->calcArea(Mouse::Pos().x, Mouse::Pos().y);
	if (0 <= here.second && here.second < a.size()) {
		str = std::to_string(a[here.second]);
		str = str.substr(0, 5);
		font(L"a["+Widen(std::to_string(here.second)) + L"]").draw(Mouse::Pos().x + 15, Mouse::Pos().y - 15);
		font(Widen(str)).draw(Mouse::Pos().x + 15, Mouse::Pos().y + 15);
	}
}



void Config::convolution() {

	if (sound_base.isEmpty())return;

	wave_base = sound_base.getWave();
	wave_fir = sound_base.getWave();
	double amp;
	std::vector<double>wav(wave_base.lengthSample, 0);

	// Convolution
	for (long i = 0; i < wave_base.lengthSample; i++) {
		amp = 0.0;
		wav[i] = wave_base[i].left / 32768.0;
		for (long j = 0; j < a.size(); j++) {
			//if (0 <= i - j)amp += a[j] * wav[i - j]; // IIR
			if (0 <= i - j)amp += a[j] * wave_base[i - j].left / 32768.0; // FIR

		}
		wav[i] = amp;
	}

	// isNormalization
	if (normalization) {
		double max = *std::max_element(wav.begin(), wav.end());
		for (unsigned long i = 0; i < wav.size(); i++) {
			wav[i] /= max;
			wave_fir[i] = Waving::DoubleToSample(wav[i]);
		}
	}
	else	for (unsigned long i = 0; i < wav.size(); i++)wave_fir[i] = Waving::DoubleToSample(wav[i]);


	wave_fir.saveWAVE(L"fir.wav");
	sound_fir = Sound(L"fir.wav");

}