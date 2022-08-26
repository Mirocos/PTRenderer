//
// Created by zeming on 2022/8/13.
//
#include "BVH.h"

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitives>> &p, int maxPrimsInNode,
                   BVHAccel::SplitMethod splitMethod)
                   : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod), primitives(p){
    time_t start, stop;
    time(&start);

    if(primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
            "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
            hrs, mins, secs);
}

std::shared_ptr<BVHBuildNode> BVHAccel::recursiveBuild(std::vector<std::shared_ptr<Primitives>> objects) {

    std::shared_ptr<BVHBuildNode> node = std::make_shared<BVHBuildNode>();
    Bound3 bounds;
    for(int i = 0; i < objects.size(); ++i){
        bounds = Bound3::Union(bounds, objects[i]->getBounds());
    }

    // only leaf
    if(objects.size() == 1){
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;

        return node;
//        node->area = objects[0]->getArea();
    }
    else if(objects.size() == 2){
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Bound3::Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        Bound3 centroidBounds;
        for(int i = 0; i < objects.size(); ++i){
            centroidBounds = Bound3::Union(centroidBounds, objects[i]->getBounds().centroid());
        }
        int dim = centroidBounds.maxExtent();
        switch (dim) {
            case 0:
                std::sort(objects.begin(), objects.end(), [](const std::shared_ptr<Primitives>& f1, const std::shared_ptr<Primitives>& f2){
                    return f1->getBounds().centroid().x < f2->getBounds().centroid().x;
                });
                break;
            case 1:
                std::sort(objects.begin(), objects.end(), [](const std::shared_ptr<Primitives>& f1, const std::shared_ptr<Primitives>& f2){
                    return f1->getBounds().centroid().y < f2->getBounds().centroid().y;
                });
                break;
            case 2:
                std::sort(objects.begin(), objects.end(), [](const std::shared_ptr<Primitives>& f1, const std::shared_ptr<Primitives>& f2){
                    return f1->getBounds().centroid().z < f2->getBounds().centroid().z;
                });
                break;
        }

        auto beginning = objects.begin();
        auto middling = beginning + (objects.size() / 2);
        auto ending = objects.end();

        auto left_shapes = std::vector<std::shared_ptr<Primitives>>(beginning, middling);
        auto right_shapes = std::vector<std::shared_ptr<Primitives>>(middling, ending);
        node->left = recursiveBuild(left_shapes);
        node->right = recursiveBuild(right_shapes);
        node->bounds = Bound3::Union(node->left->bounds, node->right->bounds);

    }

    return node;

}

Intersection BVHAccel::Intersect(const Ray &ray) const {
    Intersection isect;
    if(!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(const std::shared_ptr<BVHBuildNode> &node, const Ray &ray) const {
    auto direction = ray.get_direction();
    std::array<int, 3> isDirNeg{direction.x > 0, direction.y > 0, direction.z > 0};
    if(node->bounds.intersectBound(ray, ray.get_direction_inv(), isDirNeg)){
        if(node->left && node->right ){
            Intersection left_hit = getIntersection(node->left, ray);
            Intersection right_hit = getIntersection(node->right, ray);
            return (left_hit.get_t() < right_hit.get_t()? left_hit : right_hit);
        } else {
            Intersection intersection;
            node->object->intersect(ray, intersection, 0.f);
            return intersection;
        }
    } else {
        return Intersection();
    }

}

bool BVHAccel::intersectBound(const Ray &ray) const {
    return false;
}
