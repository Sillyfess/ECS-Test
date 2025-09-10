"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.System = void 0;
const Component_1 = require("./Component");
class System {
    constructor() {
        this.requiredComponents = [];
        this.componentMask = 0n;
        this.priority = 0;
        this.enabled = true;
    }
    setWorld(world) {
        this.world = world;
        this.updateComponentMask();
    }
    setRequiredComponents(components) {
        this.requiredComponents = components;
        this.updateComponentMask();
    }
    updateComponentMask() {
        this.componentMask = 0n;
        for (const componentClass of this.requiredComponents) {
            const typeId = Component_1.Component.getTypeId(componentClass);
            this.componentMask |= (1n << BigInt(typeId));
        }
    }
    matchesEntity(entity) {
        if (!this.enabled || this.componentMask === 0n) {
            return false;
        }
        return (entity.getComponentMask() & this.componentMask) === this.componentMask;
    }
    getEntities() {
        if (!this.world)
            return [];
        return this.world.entityManager.getEntitiesWithComponentMask(this.componentMask);
    }
    preUpdate(deltaTime) { }
    postUpdate(deltaTime) { }
}
exports.System = System;
//# sourceMappingURL=System.js.map