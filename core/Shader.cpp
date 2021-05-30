#include <Shader.h>

namespace aba {
	float lerp(float v1,float v2,const float factor) {
		return (1.0f - factor) * v1 + factor * v2;
	}

	Vec3f lerp(Vec3f v1, Vec3f v2, const float factor) {
		return (1.0f - factor) * v1 + factor * v2;
	}

	//Bresenham's line algorithm
	std::vector<Vertex> getLinePoints(Vertex v0, Vertex v1,bool is_Transpose) {
		std::vector<Vertex> points;// ascending order by the X - coordinate
		if (v0.x == v1.x && v0.y == v1.y) {
			points.push_back(v1);
			return points;
		}
		int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y;
		float z0 = v0.z, z1 = v1.z;
		bool transpose = false;
		// transpose image make disX > disY
		if (is_Transpose && abs(x1 - x0) < abs(y1 - y0)) {
			std::swap(x0, y0);
			std::swap(x1, y1);
			transpose = true;
		}
		// make it left to right
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(v0, v1);
		}
		int dx = x1 - x0, dy = abs(y1 - y0);
		int delta_erro_y = 2 * dy;
		int error_y = 0;
		float y_float = static_cast<float>(y0); // isTranspose
		int y = y0;
		float delata = (dx == 0 ? 1 : (dy * 1.0f) / (dx * 1.0f));
		Vertex vertex = v0;
		for (int x = x0; x <= x1; x++) {
			//std::cout << vertex.light_intensity << "\n";
			error_y += delta_erro_y;
			vertex.x = x;
			if (transpose) {
				vertex.x = y;
				vertex.y = x;
				points.push_back(vertex);
			}
			else { // isTranspose
				if (!is_Transpose && dx < dy) y = static_cast<int>(y_float);// isTranspose
				vertex.x = x;
				vertex.y = y;
				if ((int)vertex.x == 471 && (int)vertex.y == 199) {
					printf("");
				}
				points.push_back(vertex);
			}
			if (error_y > dx) {
				if (dx >= dy) {
					y += (y0 < y1 ? 1 : -1);
				}
				else if (!is_Transpose) { // isTranspose
					y_float += (y0 < y1 ? delata : -delata);
				}
				error_y -= 2 * dx;
			}
			float weight = dx < 0.1 ? 0 : ((x-x0+1)*1.0f / dx*1.0f);
			vertex.light_intensity = lerp(v0.light_intensity,v1.light_intensity, weight);
			vertex.z = lerp(v0.z, v1.z, weight);
			vertex.uv = lerp(v0.uv, v1.uv, weight);
			vertex.normal = lerp(v0.normal, v1.normal, weight);
		}
		return points;
	}

	//use Edge-walking,Edge equations not implement.
	void drawTriangle(Vertex* vetexs,Shader* shader, TGAImage& image, ZBuffer* z_buffer){
		Vertex v0 = vetexs[0], v1 = vetexs[1], v2 = vetexs[2];
		if (v0.y == v1.y && v1.y == v2.y) return;
		// v0,v1,v2 are sorted in ascending order by the X-coordinate (left to right)
		if (v0.x > v1.x) std::swap(v0, v1);
		if (v0.x > v2.x) std::swap(v0, v2);
		if (v1.x > v2.x) std::swap(v1, v2);

		// v0->v2 points
		std::vector<Vertex> points = getLinePoints(v0, v2, false);
		std::vector<Vertex> draw_points;
		std::vector<Vertex> t_draw_points;
		int start = 0;
		// left part v0->v1
		if (v0.x != v1.x) {
			std::vector<Vertex> left_points = getLinePoints(v0, v1, false);
			int i = 0;
			while (i < left_points.size()) {
				t_draw_points = getLinePoints(points[start], left_points[i], true);
				start++, i++;
				draw_points.insert(
					draw_points.end(),
					std::make_move_iterator(t_draw_points.begin()),
					std::make_move_iterator(t_draw_points.end())
				);
			}
			start -= 1;
		}
		// right part v1->v2
		if (v1.x != v2.x) {
			std::vector<Vertex> right_points = getLinePoints(v1, v2, false);
			int i = 0;
			while (i < right_points.size()) {
				t_draw_points = getLinePoints(points[start], right_points[i], true);
				start++, i++;
				draw_points.insert(
					draw_points.end(),
					std::make_move_iterator(t_draw_points.begin()),
					std::make_move_iterator(t_draw_points.end())
				);
			}
		}

		//draw the pixel
		for (const auto& point : draw_points) {
			int x = point.x, y = point.y, z = point.z;
			if (x < 0 || y < 0 || x >= image.getWidth() || y >= image.getHeight()) continue;
			if (z_buffer != NULL) {
				if (z_buffer->isCulling(x, y, z)) continue;
				z_buffer->set(x, y, z);
				//image.set(x, y, Color(255,255,255));
			}
			Color color;
			shader->fragmentShade(point, color);
			image.set(x, y, color);
		}
	}

	void drawWireframe(Vertex* vetexs,Color& color,TGAImage& image, ZBuffer* z_buffer) {
		Vertex v0 = vetexs[0], v1 = vetexs[1], v2 = vetexs[2];
		if (v0.y == v1.y && v1.y == v2.y) return;
		std::vector<Vertex> draw_points = getLinePoints(v0, v2, false);
		std::vector<Vertex> left_points = getLinePoints(v0, v1, false);
		std::vector<Vertex> right_points = getLinePoints(v1, v2, false);
		draw_points.insert(
			draw_points.end(),
			std::make_move_iterator(left_points.begin()),
			std::make_move_iterator(left_points.end())
		);
		draw_points.insert(
			draw_points.end(),
			std::make_move_iterator(right_points.begin()),
			std::make_move_iterator(right_points.end())
		);

		//draw the pixel
		for (const auto& point : draw_points) {
			int x = point.x, y = point.y, z = point.z;
			if (x < 0 || y < 0 || x >= image.getWidth() || y >= image.getHeight()) continue;
			if (z_buffer != NULL) {
				if (z_buffer->isCulling(x, y, z)) continue;
				z_buffer->set(x, y, z);
			}
			image.set(x, y, color);
		}
	}
}