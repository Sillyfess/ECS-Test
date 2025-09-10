"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.BoundsSystem = void 0;
const System_1 = require("../System");
const Transform_1 = require("../components/Transform");
const Velocity_1 = require("../components/Velocity");
class BoundsSystem extends System_1.System {
    constructor() {
        super();
        this.bounds = {
            minX: -1000,
            maxX: 1000,
            minY: -1000,
            maxY: 1000,
            minZ: -1000,
            maxZ: 1000
        };
        this.setRequiredComponents([Transform_1.Transform, Velocity_1.Velocity]);
        this.priority = 90;
    }
    setBounds(minX, maxX, minY, maxY, minZ, maxZ) {
        this.bounds = { minX, maxX, minY, maxY, minZ, maxZ };
    }
    update(deltaTime) {
        const entities = this.getEntities();
        for (const entity of entities) {
            const transform = this.world.getComponent(entity, Transform_1.Transform);
            const velocity = this.world.getComponent(entity, Velocity_1.Velocity);
            if (transform && velocity) {
                if (transform.x <= this.bounds.minX || transform.x >= this.bounds.maxX) {
                    velocity.dx = -velocity.dx;
                    transform.x = Math.max(this.bounds.minX, Math.min(this.bounds.maxX, transform.x));
                }
                if (transform.y <= this.bounds.minY || transform.y >= this.bounds.maxY) {
                    velocity.dy = -velocity.dy;
                    transform.y = Math.max(this.bounds.minY, Math.min(this.bounds.maxY, transform.y));
                }
                if (transform.z <= this.bounds.minZ || transform.z >= this.bounds.maxZ) {
                    velocity.dz = -velocity.dz;
                    transform.z = Math.max(this.bounds.minZ, Math.min(this.bounds.maxZ, transform.z));
                }
            }
        }
    }
}
exports.BoundsSystem = BoundsSystem;
//# sourceMappingURL=BoundsSystem.js.map