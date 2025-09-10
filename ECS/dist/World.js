"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.World = void 0;
const EntityManager_1 = require("./EntityManager");
const ComponentManager_1 = require("./ComponentManager");
const SystemManager_1 = require("./SystemManager");
class World {
    constructor() {
        this.lastUpdateTime = 0;
        this.entityManager = new EntityManager_1.EntityManager();
        this.componentManager = new ComponentManager_1.ComponentManager();
        this.systemManager = new SystemManager_1.SystemManager();
        this.systemManager.setWorld(this);
    }
    createEntity() {
        return this.entityManager.createEntity();
    }
    destroyEntity(entityId) {
        const entity = this.entityManager.getEntity(entityId);
        if (entity) {
            this.componentManager.clearEntityComponents(entity);
            this.entityManager.destroyEntity(entityId);
        }
    }
    addComponent(entity, componentClass, component) {
        this.componentManager.addComponent(entity, componentClass, component);
    }
    getComponent(entity, componentClass) {
        return this.componentManager.getComponent(entity, componentClass);
    }
    removeComponent(entity, componentClass) {
        this.componentManager.removeComponent(entity, componentClass);
    }
    hasComponent(entity, componentClass) {
        return this.componentManager.hasComponent(entity, componentClass);
    }
    addSystem(system) {
        this.systemManager.addSystem(system);
    }
    removeSystem(system) {
        this.systemManager.removeSystem(system);
    }
    getSystem(systemClass) {
        return this.systemManager.getSystem(systemClass);
    }
    update(currentTime) {
        if (currentTime === undefined) {
            currentTime = Date.now();
        }
        const deltaTime = this.lastUpdateTime === 0
            ? 16.67
            : currentTime - this.lastUpdateTime;
        this.systemManager.update(deltaTime / 1000);
        this.lastUpdateTime = currentTime;
    }
    clear() {
        this.systemManager.clear();
        this.componentManager.clear();
        this.entityManager.clear();
        this.lastUpdateTime = 0;
    }
    getEntityCount() {
        return this.entityManager.getEntityCount();
    }
    getStats() {
        return {
            entityCount: this.entityManager.getEntityCount(),
            systemCount: this.systemManager.getSystems().length,
            activeEntities: this.entityManager.getActiveEntities().length
        };
    }
}
exports.World = World;
//# sourceMappingURL=World.js.map