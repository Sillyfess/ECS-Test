"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Entity = void 0;
class Entity {
    constructor() {
        this.componentMask = 0n;
        this.alive = true;
        this.id = Entity.nextId++;
    }
    setComponentMask(mask) {
        this.componentMask = mask;
    }
    getComponentMask() {
        return this.componentMask;
    }
    addComponentBit(componentTypeId) {
        this.componentMask |= (1n << BigInt(componentTypeId));
    }
    removeComponentBit(componentTypeId) {
        this.componentMask &= ~(1n << BigInt(componentTypeId));
    }
    hasComponentBit(componentTypeId) {
        return (this.componentMask & (1n << BigInt(componentTypeId))) !== 0n;
    }
    destroy() {
        this.alive = false;
        this.componentMask = 0n;
    }
    isAlive() {
        return this.alive;
    }
    reset() {
        this.componentMask = 0n;
        this.alive = true;
    }
}
exports.Entity = Entity;
Entity.nextId = 0;
//# sourceMappingURL=Entity.js.map