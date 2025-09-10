import { Entity } from './Entity';
import { Component } from './Component';
import { World } from './World';

export abstract class System {
    protected world!: World;
    protected requiredComponents: (new (...args: any[]) => Component)[] = [];
    protected componentMask: bigint = 0n;
    public priority: number = 0;
    public enabled: boolean = true;

    public setWorld(world: World): void {
        this.world = world;
        this.updateComponentMask();
    }

    protected setRequiredComponents(components: (new (...args: any[]) => Component)[]): void {
        this.requiredComponents = components;
        this.updateComponentMask();
    }

    private updateComponentMask(): void {
        this.componentMask = 0n;
        for (const componentClass of this.requiredComponents) {
            const typeId = Component.getTypeId(componentClass);
            this.componentMask |= (1n << BigInt(typeId));
        }
    }

    public matchesEntity(entity: Entity): boolean {
        if (!this.enabled || this.componentMask === 0n) {
            return false;
        }
        return (entity.getComponentMask() & this.componentMask) === this.componentMask;
    }

    public getEntities(): Entity[] {
        if (!this.world) return [];
        return this.world.entityManager.getEntitiesWithComponentMask(this.componentMask);
    }

    public abstract update(deltaTime: number): void;

    public preUpdate(deltaTime: number): void {}
    
    public postUpdate(deltaTime: number): void {}
}