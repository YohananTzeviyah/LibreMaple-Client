//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "boost/container/static_vector.hpp"

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace jrc
{
template<typename K, typename V>
class QuadTree
{
public:
    enum Direction { LEFT, RIGHT, UP, DOWN };

    QuadTree(std::function<Direction(const V&, const V&)> c)
        : comparator(c), root(0)
    {
    }

    QuadTree() : QuadTree(nullptr)
    {
    }

    void clear()
    {
        nodes.clear();

        root = 0;
    }

    void add(K key, V value)
    {
        K parent = 0;
        if (root) {
            K current = root;
            while (current) {
                parent = current;
                current = nodes[parent].add_or_next(key, value, comparator);
            }
        } else {
            root = key;
        }

        nodes.emplace(std::piecewise_construct,
                      std::forward_as_tuple(key),
                      std::forward_as_tuple(value, parent, 0, 0, 0, 0));
    }

    void erase(K key)
    {
        if (!nodes.count(key)) {
            return;
        }

        Node& to_erase = nodes[key];

        boost::container::static_vector<K, 4> leaves;
        for (std::size_t i = LEFT; i <= DOWN; ++i) {
            if (K leaf_key = to_erase[i]; leaf_key) {
                leaves.push_back(std::move(leaf_key));
            }
        }

        K parent = to_erase.parent;
        if (root == key) {
            root = 0;
        } else if (nodes.count(parent)) {
            nodes[parent].erase(key);
        }

        nodes.erase(key);

        for (auto& leaf : leaves) {
            re_add(parent, leaf);
        }
    }

    K find_node(const V& value,
                std::function<bool(const V&, const V&)> predicate)
    {
        if (root) {
            K key = find_from(root, value, predicate);
            return predicate(value, nodes[key].value) ? key : 0;
        } else {
            return 0;
        }
    }

    V& operator[](K key)
    {
        return nodes[key].value;
    }

    const V& operator[](K key) const
    {
        return nodes.at(key).value;
    }

private:
    K find_from(K start,
                const V& value,
                std::function<bool(const V&, const V&)> predicate)
    {
        if (!start) {
            return 0;
        }

        bool fulfilled = predicate(value, nodes[start].value);
        Direction dir = comparator(value, nodes[start].value);
        if (dir == RIGHT) {
            K right = find_from(nodes[start].right, value, predicate);
            if (right && predicate(value, nodes[right].value)) {
                return right;
            } else {
                return start;
            }
        } else if (dir == DOWN) {
            K bottom = find_from(nodes[start].bottom, value, predicate);
            if (bottom && predicate(value, nodes[bottom].value)) {
                return bottom;
            } else if (fulfilled) {
                return start;
            } else {
                K right = find_from(nodes[start].right, value, predicate);
                if (right && predicate(value, nodes[right].value)) {
                    return right;
                } else {
                    return start;
                }
            }
        } else if (dir == UP) {
            K top = find_from(nodes[start].top, value, predicate);
            if (top && predicate(value, nodes[top].value)) {
                return top;
            } else if (fulfilled) {
                return start;
            } else {
                K right = find_from(nodes[start].right, value, predicate);
                if (right && predicate(value, nodes[right].value)) {
                    return right;
                } else {
                    return start;
                }
            }
        } else {
            K left = find_from(nodes[start].left, value, predicate);
            if (left && predicate(value, nodes[left].value)) {
                return left;
            } else if (fulfilled) {
                return start;
            }

            K bottom = find_from(nodes[start].bottom, value, predicate);
            if (bottom && predicate(value, nodes[bottom].value)) {
                return bottom;
            }

            K top = find_from(nodes[start].top, value, predicate);
            if (top && predicate(value, nodes[top].value)) {
                return top;
            }

            K right = find_from(nodes[start].right, value, predicate);
            if (right && predicate(value, nodes[right].value)) {
                return right;
            } else {
                return start;
            }
        }
    }

    void re_add(K start, K key)
    {
        if (start) {
            K parent = 0;
            K current = start;
            while (current) {
                parent = current;
                current = nodes[parent].add_or_next(
                    key, nodes[key].value, comparator);
            }

            nodes[key].parent = parent;
        } else if (start == root) {
            root = key;

            nodes[key].parent = 0;
        } else if (root) {
            re_add(root, key);
        }
    }

    struct Node {
        V value;
        K parent;
        K left;
        K right;
        K top;
        K bottom;

        Node(const V& v, K p, K l, K r, K t, K b)
            : value(v), parent(p), left(l), right(r), top(t), bottom(b)
        {
        }

        Node() : Node(V(), 0, 0, 0, 0, 0)
        {
        }

        void erase(K key)
        {
            if (left == key) {
                left = 0;
            } else if (right == key) {
                right = 0;
            } else if (top == key) {
                top = 0;
            } else if (bottom == key) {
                bottom = 0;
            }
        }

        K add_or_next(K key,
                      V val,
                      std::function<Direction(const V&, const V&)> cmptor)
        {
            Direction dir = cmptor(val, value);
            K dirkey = leaf(dir);
            if (!dirkey) {
                switch (dir) {
                case LEFT:
                    left = key;
                    break;
                case RIGHT:
                    right = key;
                    break;
                case UP:
                    top = key;
                    break;
                case DOWN:
                    bottom = key;
                    break;
                }
            }
            return dirkey;
        }

        K leaf(Direction dir)
        {
            switch (dir) {
            case LEFT:
                return left;
            case RIGHT:
                return right;
            case UP:
                return top;
            case DOWN:
                return bottom;
            default:
                return 0;
            }
        }

        K operator[](std::size_t d)
        {
            auto dir = static_cast<Direction>(d);
            return leaf(dir);
        }
    };

    std::function<Direction(const V&, const V&)> comparator;
    std::unordered_map<K, Node> nodes;
    K root;
};
} // namespace jrc
