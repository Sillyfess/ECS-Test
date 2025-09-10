"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.MovementSystem = void 0;
const System_1 = require("../System");
const Transform_1 = require("../components/Transform");
const Velocity_1 = require("../components/Velocity");
class MovementSystem extends System_1.System {
    constructor() {
        super();
        this.setRequiredComponents([Transform_1.Transform, Velocity_1.Velocity]);
        this.priority = 100;
    }
    update(deltaTime) {
        const entities = this.getEntities();
        for (const entity of entities) {
            const transform = this.world.getComponent(entity, Transform_1.Transform);
            const velocity = this.world.getComponent(entity, Velocity_1.Velocity);
            if (transform && velocity) {
                transform.x += velocity.dx * deltaTime;
                transform.y += velocity.dy * deltaTime;
                transform.z += velocity.dz * deltaTime;
            }
        }
    }
}
exports.MovementSystem = MovementSystem;
//# sourceMappingURL=MovementSystem.js.map