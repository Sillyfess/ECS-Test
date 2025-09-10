"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.SystemManager = void 0;
class SystemManager {
    constructor() {
        this.systems = [];
    }
    setWorld(world) {
        this.world = world;
    }
    addSystem(system) {
        system.setWorld(this.world);
        this.systems.push(system);
        this.sortSystems();
    }
    removeSystem(system) {
        const index = this.systems.indexOf(system);
        if (index !== -1) {
            this.systems.splice(index, 1);
        }
    }
    getSystem(systemClass) {
        return this.systems.find(s => s instanceof systemClass);
    }
    sortSystems() {
        this.systems.sort((a, b) => b.priority - a.priority);
    }
    update(deltaTime) {
        for (const system of this.systems) {
            if (system.enabled) {
                system.preUpdate(deltaTime);
                system.update(deltaTime);
                system.postUpdate(deltaTime);
            }
        }
    }
    clear() {
        this.systems = [];
    }
    getSystems() {
        return [...this.systems];
    }
}
exports.SystemManager = SystemManager;
//# sourceMappingURL=SystemManager.js.map