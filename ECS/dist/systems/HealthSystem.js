"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.HealthSystem = void 0;
const System_1 = require("../System");
const Health_1 = require("../components/Health");
class HealthSystem extends System_1.System {
    constructor() {
        super();
        this.entitiesToDestroy = [];
        this.setRequiredComponents([Health_1.Health]);
        this.priority = 50;
    }
    update(deltaTime) {
        const entities = this.getEntities();
        for (const entity of entities) {
            const health = this.world.getComponent(entity, Health_1.Health);
            if (health && health.isDead()) {
                this.entitiesToDestroy.push(entity.id);
            }
        }
    }
    postUpdate(deltaTime) {
        for (const entityId of this.entitiesToDestroy) {
            this.world.destroyEntity(entityId);
        }
        this.entitiesToDestroy = [];
    }
}
exports.HealthSystem = HealthSystem;
//# sourceMappingURL=HealthSystem.js.map