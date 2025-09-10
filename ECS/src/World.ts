import { EntityManager } from './EntityManager';
import { ComponentManager } from './ComponentManager';
import { SystemManager } from './SystemManager';
import { Entity } from './Entity';
import { Component } from './Component';
import { System } from './System';

export class World {
    public readonly entityManager: EntityManager;
    public readonly componentManager: ComponentManager;
    public readonly systemManager: SystemManager;
    
    private lastUpdateTime: number = 0;

    constructor() {
        this.entityManager = new EntityManager();
        this.componentManager = new ComponentManager();
        this.systemManager = new SystemManager();
        this.systemManager.setWorld(this);
    }

    public createEntity(): Entity {
        return this.entityManager.createEntity();
    }

    public destroyEntity(entityId: number): void {
        const entity = this.entityManager.getEntity(entityId);
        if (entity) {
            this.componentManager.clearEntityComponents(entity);
            this.entityManager.destroyEntity(entityId);
        }
    }

    public addComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T,
        component: T
    ): void {
        this.componentManager.addComponent(entity, componentClass, component);
    }

    public getComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): T | undefined {
        return this.componentManager.getComponent(entity, componentClass);
    }

    public removeComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): void {
        this.componentManager.removeComponent(entity, componentClass);
    }

    public hasComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): boolean {
        return this.componentManager.hasComponent(entity, componentClass);
    }

    public addSystem(system: System): void {
        this.systemManager.addSystem(system);
    }

    public removeSystem(system: System): void {
        this.systemManager.removeSystem(system);
    }

    public getSystem<T extends System>(systemClass: new (...args: any[]) => T): T | undefined {
        return this.systemManager.getSystem(systemClass);
    }

    public update(currentTime?: number): void {
        if (currentTime === undefined) {
            currentTime = Date.now();
        }

        const deltaTime = this.lastUpdateTime === 0 
            ? 16.67 
            : currentTime - this.lastUpdateTime;
        
        this.systemManager.update(deltaTime / 1000);
        this.lastUpdateTime = currentTime;
    }

    public clear(): void {
        this.systemManager.clear();
        this.componentManager.clear();
        this.entityManager.clear();
        this.lastUpdateTime = 0;
    }

    public getEntityCount(): number {
        return this.entityManager.getEntityCount();
    }

    public getStats(): {
        entityCount: number;
        systemCount: number;
        activeEntities: number;
    } {
        return {
            entityCount: this.entityManager.getEntityCount(),
            systemCount: this.systemManager.getSystems().length,
            activeEntities: this.entityManager.getActiveEntities().length
        };
    }
}