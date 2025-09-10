"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Component = void 0;
class Component {
    static getTypeId(componentClass) {
        if (!this.typeIds.has(componentClass)) {
            this.typeIds.set(componentClass, this.typeIdCounter++);
        }
        return this.typeIds.get(componentClass);
    }
}
exports.Component = Component;
Component.typeIdCounter = 0;
Component.typeIds = new Map();
//# sourceMappingURL=Component.js.map