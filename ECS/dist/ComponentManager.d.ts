import { Component } from './Component';
import { Entity } from './Entity';
export declare class ComponentManager {
    private componentPools;
    private componentTypes;
    registerComponent<T extends Component>(componentClass: new (...args: any[]) => T): void;
    addComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T, component: T): void;
    getComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): T | undefined;
    removeComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): void;
    hasComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): boolean;
    getAllComponents<T extends Component>(componentClass: new (...args: any[]) => T): T[];
    clearEntityComponents(entity: Entity): void;
    clear(): void;
}
//# sourceMappingURL=ComponentManager.d.ts.map