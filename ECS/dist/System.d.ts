import { Entity } from './Entity';
import { Component } from './Component';
import { World } from './World';
export declare abstract class System {
    protected world: World;
    protected requiredComponents: (new (...args: any[]) => Component)[];
    protected componentMask: bigint;
    priority: number;
    enabled: boolean;
    setWorld(world: World): void;
    protected setRequiredComponents(components: (new (...args: any[]) => Component)[]): void;
    private updateComponentMask;
    matchesEntity(entity: Entity): boolean;
    getEntities(): Entity[];
    abstract update(deltaTime: number): void;
    preUpdate(deltaTime: number): void;
    postUpdate(deltaTime: number): void;
}
//# sourceMappingURL=System.d.ts.map