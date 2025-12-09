#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;


enum NORMAL_DIRECTION {
    TOP = 0b00,
    LEFT = 0b01,
    BOTTOM = 0b10,
    RIGHT = 0b11,
};

class Coordinate {
    public:
        int x, y;

        Coordinate() {
            x = 0;
            y = 0;
        }
        Coordinate(int x_pos, int y_pos) {
            x = x_pos;
            y = y_pos;
        }

        string to_string() const {
            return "Coord(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        bool operator == (const Coordinate& c) const {
            return x == c.x && y == c.y;
        }
};

class Edge {
    private:
        Coordinate c1, c2;
    
    public:
        int normal = TOP;
        
        Edge() {}
        Edge(Coordinate coord1, Coordinate coord2, NORMAL_DIRECTION norm = TOP) {
            c1 = coord1;
            c2 = coord2;
            normal = norm;
        }

        string to_string() const {
            return "Edge[" + c1.to_string() + ", " + c2.to_string() + "]";
        }

        void testlog(bool vertical, int num, Edge e) {
            return;
            cout << (vertical ? "v" : "h") + std::to_string(num) + " r: " << to_string() << " p: " << e.to_string() << endl;
        }

        bool intersects(Edge edge) {
            if (is_vertical() == edge.is_vertical()) return false;

            Coordinate p_c1 = edge.c1, p_c2 = edge.c2;
            Coordinate r_c1 = c1, r_c2 = c2;

            if (p_c1.x > p_c2.x) {
                swap(p_c1.x, p_c2.x);
            }
            if (p_c1.y > p_c2.y) {
                swap(p_c1.y, p_c2.y);
            }
            if (r_c1.x > r_c2.x) {
                swap(r_c1.x, r_c2.x);
            }
            if (r_c1.y > r_c2.y) {
                swap(r_c1.y, r_c2.y);
            }

            if (is_vertical()) {
                // N_Edge not within Edge's vertical bounds
                if (p_c1.y < r_c1.y || r_c2.y < p_c1.y) {
                    testlog(true, 1, edge);
                    return false;
                }

                // Fully Intersects
                if (p_c1.x < r_c1.x && r_c1.x < p_c2.x) {
                    testlog(true, 2, edge);
                    return true;
                }

                // Touches at one point
                if (p_c1.x == r_c1.x) {
                    if (normal == LEFT && p_c1.x != p_c2.x) {
                        if (p_c1.y == r_c1.y) {
                            return edge.normal != TOP;
                        } else if (p_c1.y == r_c2.y) {
                            return edge.normal != BOTTOM;
                        }

                        testlog(true, 3, edge);
                        return true;
                    }
                } else if (p_c2.x == r_c1.x) {
                    if (normal == RIGHT && p_c1.x != p_c2.x) {
                        if (p_c1.y == r_c1.y) {
                            return edge.normal != TOP;
                        } else if (p_c1.y == r_c2.y) {
                            return edge.normal != BOTTOM;
                        }
                        
                        testlog(true, 4, edge);
                        return true;
                    }
                }
            } else {
                // N_Edge not within Edge's horizontal bounds
                if (p_c1.x < r_c1.x || r_c2.x < p_c1.x) {
                    testlog(false, 1, edge);
                    return false;
                }

                // Fully Intersects
                if (p_c1.y < r_c1.y && r_c2.y < p_c2.y) {
                    testlog(false, 2, edge);
                    return true;
                }

                // Touches at one point
                if (p_c1.y == r_c1.y) {
                    if (normal == TOP && p_c1.y != p_c2.y) {
                        if (p_c1.x == r_c1.x) {
                            return edge.normal != LEFT;
                        } else if (p_c1.x == r_c2.x) {
                            return edge.normal != RIGHT;
                        }

                        testlog(false, 3, edge);
                        return true;
                    }
                } else if (p_c2.y == r_c1.y) {
                    if (normal == BOTTOM && p_c1.y != p_c2.y) {
                        if (p_c1.x == r_c1.x) {
                            return edge.normal != LEFT;
                        } else if (p_c1.x == r_c2.x) {
                            return edge.normal != RIGHT;
                        }

                        testlog(false, 4, edge);
                        return true;
                    }
                }
            }

            //cout << "n p:" << to_string() << " r:" << edge.to_string() << endl;
            return false;
        }
        bool is_vertical() {
            return c1.x == c2.x;
        }
 
        Coordinate get_c1() const {
            return c1;
        }
        Coordinate get_c2() const {
            return c2;
        }

        bool operator == (const Edge& e) const {
            return c1 == e.c1 && c2 == e.c2;
        }
};

class Rect {
    private:
        Coordinate c1, c2;
        size_t area;
    
    public:
        Rect() {area = 1;}
        Rect(Coordinate coord1, Coordinate coord2) {
            c1 = coord1;
            c2 = coord2;

            if (coord1.x > coord2.x) {
                swap(coord1.x, coord2.x);
            }
            if (coord1.y > coord2.y) {
                swap(coord1.y, coord2.y);
            }
            area = (coord2.x - coord1.x + 1) * (coord2.y - coord1.y + 1);
        }

        string to_string() const {
            return "Rect[" + c1.to_string() + ", " + c2.to_string() + "]";
        }

        size_t get_area() const {
            return area;
        }
        vector<Edge> get_edges() const {
            vector<Edge> edges;

            Coordinate min_coord = c1;
            Coordinate max_coord = c2;
            if (min_coord.x > max_coord.x) {
                swap(min_coord.x, max_coord.x);
            }
            if (min_coord.y > max_coord.y) {
                swap(min_coord.y, max_coord.y);
            }

            // False = Bottom/Right, True = Top/Left
            edges.push_back(Edge(
                Coordinate(min_coord.x, min_coord.y),
                Coordinate(min_coord.x, max_coord.y),
                LEFT
            ));
            edges.push_back(Edge(
                Coordinate(min_coord.x, max_coord.y),
                Coordinate(max_coord.x, max_coord.y),
                BOTTOM
            ));
            edges.push_back(Edge(
                Coordinate(max_coord.x, max_coord.y),
                Coordinate(max_coord.x, min_coord.y),
                RIGHT
            ));
            edges.push_back(Edge(
                Coordinate(max_coord.x, min_coord.y),
                Coordinate(min_coord.x, min_coord.y),
                TOP
            ));
            
            return edges;
        }
 
        Coordinate get_c1() const {
            return c1;
        }
        Coordinate get_c2() const {
            return c2;
        }


        bool operator == (const Rect& r) const {
            return c1 == r.c1 && c2 == r.c2;
        }
        bool operator < (const Rect& rect) const {
            return get_area() < rect.get_area();
        }
}; 


class Polygon {
    private:
        vector<Coordinate> vertices;

    public:
        NORMAL_DIRECTION angle_normal(NORMAL_DIRECTION normal, Coordinate s, Coordinate e) const {
            int dx = s.x - e.x;
            int dy = s.y - e.y;

            int mod = ((dx > 0) + (dy > 0)) % 2;
            switch (normal) {
                case TOP:
                    if (mod == 0) {
                        return RIGHT;
                    } else {
                        return LEFT;
                    }
                case LEFT:
                    if (mod == 0) {
                        return BOTTOM;
                    } else {
                        return TOP;
                    }
                case BOTTOM:
                    if (mod == 0) {
                        return LEFT;
                    } else {
                        return RIGHT;
                    }
                case RIGHT:
                    if (mod == 0) {
                        return TOP;
                    } else {
                        return BOTTOM;
                    }
            }
            return TOP;
        }

        Polygon() {}
        Polygon(vector<Coordinate> point_vertices) {
            vertices = point_vertices;
        }

        bool rect_intersects(Rect rect) {
            vector<Edge> rect_edges = rect.get_edges();
            vector<Edge> poly_edges = get_edges();
            
            for(auto poly_edge : poly_edges) {
                for(auto rect_edge : rect_edges) {
                    //cout << poly_edge.to_string() << " : " << rect_edge.to_string() << endl;
                    
                    if (rect_edge.intersects(poly_edge)) return true;
                }
            }

            cout << "vaild: " << rect.to_string() << endl;
            return false;
        }

        string to_string() const {
            string ret = "Polygon<";
            for(int i = 0; i < vertices.size(); i++) {
                ret += vertices[i].to_string();

                if (i != vertices.size() - 1) {
                    ret += ", ";
                }
            }
            ret += ">";

            return ret;
        }

        vector<Edge> get_edges() const {
            vector<Edge> edges;
            auto vec = vertices;
            vec.push_back(vec[0]);

            NORMAL_DIRECTION c_normal = TOP;
            edges.push_back(Edge(
                vec[0],
                vec[1],
                c_normal
            ));
            //cout << edges.back().to_string() << " " << c_normal << endl;

            for(int i = 1; i < vec.size() - 1; i++) {
                c_normal = angle_normal(c_normal, vec[i - 1], vec[i + 1]);
                edges.push_back(Edge(
                    vec[i],
                    vec[i + 1],
                    c_normal
                ));
                //cout << edges.back().to_string() << " " << c_normal << endl;
            }
            
            return edges;
        }
};


const string FILE_NAME = "input.txt";
ifstream open_file(string filename) {
    ifstream inputStream;

    cout << endl;

    inputStream.open(filename);
    if (inputStream.fail()) {
        cout << "Failed to open file '" << filename << "'." << endl;
        exit(1);
    }
    if (!inputStream.is_open()) {
        cout << "File '" << filename << "' didn't open." << endl;
        exit(1);
    }

    cout << "File '" << filename << "' opened successfully." << endl;

    return inputStream;
}

Rect process_coordinates(vector<Coordinate> coordinates) {
    Polygon poly(coordinates);
    vector<Rect> rectangles;

    //poly.rect_intersects(Rect(Coordinate(7, 1), Coordinate(2, 3)));
    //poly.rect_intersects(Rect(Coordinate(9, 5), Coordinate(2, 3)));
    //return Rect();

    for(int idx1 = 0; idx1 < coordinates.size(); idx1++) {
        for(int idx2 = idx1 + 1; idx2 < coordinates.size(); idx2++) {
            Rect rectangle = Rect(coordinates[idx1], coordinates[idx2]);

            if (poly.rect_intersects(rectangle)) continue;

            rectangles.push_back(rectangle);
        }
    }

    sort(rectangles.begin(), rectangles.end());
    for(auto rect : rectangles) {
        cout << rect.to_string() << " " << rect.get_area() << endl;
    }
    
    if (rectangles.empty()) return Rect();
    return rectangles.back();
}


Coordinate process_line(string textline) {
    stringstream ss(textline);
    int x, y;
    char chars;

    ss >> x >> chars >> y;
    return Coordinate(x, y);
}
size_t process_file() {
    string textline;
    ifstream inputStream;
    inputStream = open_file(FILE_NAME);

    vector<Coordinate> coordinates;

    while(getline(inputStream, textline)) {
        coordinates.push_back(process_line(textline));
    }
    return process_coordinates(coordinates).get_area();
}

int main() {
    size_t output = process_file();
    cout << "The largest rectangle area is " << output << endl;
}