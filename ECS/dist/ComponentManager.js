"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.ComponentManager = void 0;
const Component_1 = require("./Component");
class SparseSet {
    constructor() {
        this.dense = [];
        this.sparse = new Map();
    }
    get(entityId) {
        const index = this.sparse.get(entityId);
        return index !== undefined ? this.dense[index] : undefined;
    }
    set(entityId, component) {
        if (this.sparse.has(entityId)) {
            const index = this.sparse.get(entityId);
            this.dense[index] = component;
        }
        else {
            const index = this.dense.length;
            this.dense.push(component);
            this.sparse.set(entityId, index);
        }
    }
    remove(entityId) {
        const index = this.sparse.get(entityId);
        if (index !== undefined) {
            const lastIndex = this.dense.length - 1;
            if (index !== lastIndex) {
                this.dense[index] = this.dense[lastIndex];
                for (const [eid, idx] of this.sparse.entries()) {
                    if (idx === lastIndex) {
                        this.sparse.set(eid, index);
                        break;
                    }
                }
            }
            this.dense.pop();
            this.sparse.delete(entityId);
        }
    }
    clear() {
        this.dense = [];
        this.sparse.clear();
    }
    getAll() {
        return this.dense.filter(c => c !== undefined);
    }
}
class ComponentManager {
    constructor() {
        this.componentPools = new Map();
        this.componentTypes = new Map();
    }
    registerComponent(componentClass) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        if (!this.componentPools.has(typeId)) {
            this.componentPools.set(typeId, new SparseSet());
            this.componentTypes.set(typeId, componentClass);
        }
    }
    addComponent(entity, componentClass, component) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        if (!this.componentPools.has(typeId)) {
            this.registerComponent(componentClass);
        }
        const pool = this.componentPools.get(typeId);
        pool.set(entity.id, component);
        entity.addComponentBit(typeId);
    }
    getComponent(entity, componentClass) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId);
        if (!pool || !entity.hasComponentBit(typeId)) {
            return undefined;
        }
        return pool.get(entity.id);
    }
    removeComponent(entity, componentClass) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId);
        if (pool && entity.hasComponentBit(typeId)) {
            pool.remove(entity.id);
            entity.removeComponentBit(typeId);
        }
    }
    hasComponent(entity, componentClass) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        return entity.hasComponentBit(typeId);
    }
    getAllComponents(componentClass) {
        const typeId = Component_1.Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId);
        return pool ? pool.getAll() : [];
    }
    clearEntityComponents(entity) {
        for (const [typeId, pool] of this.componentPools) {
            if (entity.hasComponentBit(typeId)) {
                pool.remove(entity.id);
            }
        }
    }
    clear() {
        for (const pool of this.componentPools.values()) {
            pool.clear();
        }
    }
}
exports.ComponentManager = ComponentManager;
//# sourceMappingURL=ComponentManager.js.map