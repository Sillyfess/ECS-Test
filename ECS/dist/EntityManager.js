"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.EntityManager = void 0;
const Entity_1 = require("./Entity");
class EntityManager {
    constructor() {
        this.entities = new Map();
        this.entityPool = [];
        this.poolSize = 100;
        this.initializePool();
    }
    initializePool() {
        for (let i = 0; i < this.poolSize; i++) {
            this.entityPool.push(new Entity_1.Entity());
        }
    }
    createEntity() {
        let entity;
        if (this.entityPool.length > 0) {
            entity = this.entityPool.pop();
            entity.reset();
        }
        else {
            entity = new Entity_1.Entity();
        }
        this.entities.set(entity.id, entity);
        return entity;
    }
    destroyEntity(entityId) {
        const entity = this.entities.get(entityId);
        if (entity) {
            entity.destroy();
            this.entities.delete(entityId);
            if (this.entityPool.length < this.poolSize) {
                this.entityPool.push(entity);
            }
        }
    }
    getEntity(entityId) {
        return this.entities.get(entityId);
    }
    getAllEntities() {
        return Array.from(this.entities.values());
    }
    getActiveEntities() {
        return Array.from(this.entities.values()).filter(e => e.isAlive());
    }
    getEntitiesWithComponentMask(mask) {
        const result = [];
        for (const entity of this.entities.values()) {
            if (entity.isAlive() && (entity.getComponentMask() & mask) === mask) {
                result.push(entity);
            }
        }
        return result;
    }
    clear() {
        for (const entity of this.entities.values()) {
            entity.destroy();
        }
        this.entities.clear();
    }
    getEntityCount() {
        return this.entities.size;
    }
}
exports.EntityManager = EntityManager;
//# sourceMappingURL=EntityManager.js.map