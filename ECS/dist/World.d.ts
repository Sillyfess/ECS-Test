import { EntityManager } from './EntityManager';
import { ComponentManager } from './ComponentManager';
import { SystemManager } from './SystemManager';
import { Entity } from './Entity';
import { Component } from './Component';
import { System } from './System';
export declare class World {
    readonly entityManager: EntityManager;
    readonly componentManager: ComponentManager;
    readonly systemManager: SystemManager;
    private lastUpdateTime;
    constructor();
    createEntity(): Entity;
    destroyEntity(entityId: number): void;
    addComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T, component: T): void;
    getComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): T | undefined;
    removeComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): void;
    hasComponent<T extends Component>(entity: Entity, componentClass: new (...args: any[]) => T): boolean;
    addSystem(system: System): void;
    removeSystem(system: System): void;
    getSystem<T extends System>(systemClass: new (...args: any[]) => T): T | undefined;
    update(currentTime?: number): void;
    clear(): void;
    getEntityCount(): number;
    getStats(): {
        entityCount: number;
        systemCount: number;
        activeEntities: number;
    };
}
//# sourceMappingURL=World.d.ts.map