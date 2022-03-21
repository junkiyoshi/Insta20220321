#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(1, 5);
	this->base_location_list = ico_sphere.getMesh().getVertices();

	while (this->location_list.size() < 400) {

		int index = ofRandom(this->base_location_list.size());
		auto tmp_location = this->base_location_list[index];
		tmp_location = glm::normalize(tmp_location) * ofRandom(0, 95);

		auto radius = this->location_list.size() < 110 ? ofRandom(10, 50) : ofRandom(3, 20);

		bool flag = true;
		for (int i = 0; i < this->location_list.size(); i++) {

			if (glm::distance(tmp_location, location_list[i]) < this->radius_list[i] + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			this->location_list.push_back(tmp_location);
			this->radius_list.push_back(radius);
		}
	}

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);

	for (char c = 'A'; c <= 'Z'; c++) {

		this->chara_path.push_back(font.getCharacterAsPoints(c, true, false));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	for (int i = 0; i < this->location_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->location_list[i]);

		ofRotateZ(ofMap(ofNoise(ofRandom(100), ofGetFrameNum() * 0.005), 0, 1, -360, 360));
		ofRotateY(ofMap(ofNoise(ofRandom(100), ofGetFrameNum() * 0.005), 0, 1, -360, 360));
		ofRotateX(ofMap(ofNoise(ofRandom(100), ofGetFrameNum() * 0.005), 0, 1, -360, 360));

		int chara_index = i % this->chara_path.size();
		chara_index = (int)(chara_index + ofMap(ofNoise(glm::vec4(this->location_list[i] * 0.005, ofGetFrameNum() * 0.00015)), 0, 1, 0, chara_path.size())) % chara_path.size();

		auto font_size = radius_list[i] * 1.5;
		auto outline = this->chara_path[chara_index].getOutline();

		ofFill();
		ofSetColor(255);
		ofBeginShape();
		for (int line_index = 0; line_index < outline.size(); line_index++) {

			if (line_index != 0) { ofNextContour(true); }

			auto vertices = outline[line_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto point = (vertices[vertices_index] / 100 * font_size) + glm::vec2(font_size * -0.5, font_size * 0.5);
				ofVertex(point);
			}
		}
		ofEndShape(true);

		ofNoFill();
		ofSetColor(0);
		ofBeginShape();
		for (int line_index = 0; line_index < outline.size(); line_index++) {

			if (line_index != 0) { ofNextContour(true); }

			auto vertices = outline[line_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto point = (vertices[vertices_index] / 100 * font_size) + glm::vec2(font_size * -0.5, font_size * 0.5);
				ofVertex(point);
			}
		}
		ofEndShape(true);

		ofPopMatrix();
	}

	this->cam.end();
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}